#include "Parser.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "BitMove.h"
#include "Board.h"
#include "Definitions.h"

#define DEBUG 0

Parser::Parser(Board &b, int ac, char **av) : board{b}, argc{ac}, argv{av} {
}

void Parser::parseArgs() {
    // for each argument
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg != "-H" && arg != "-m") Util::exitError("Unrecognized argument `" + arg + "`.");

        if (i + 1 < argc && argv[i + 1][0] != '-')
            argMap[arg] = argv[++i];  // store next argument
        else
            Util::exitError("Missing value for option `" + arg + "`.");
    }

    if (argMap.count("-H")) {
        historyFile = argMap["-H"];
        Util::printDebug("Input history filename: " + historyFile);
    } else {
        Util::exitError("-H option is required.");
    }

    if (argMap.count("-m")) {
        outputFile = argMap["-m"];
        Util::printDebug("Output move file: " + outputFile);
    } else {
        Util::exitError("-m option is required.");
    }
}

void Parser::parseHistory() {
    std::ifstream inputStream(historyFile);

    if (!inputStream || !inputStream.is_open()) Util::exitError("Could not open file `" + historyFile + "`.");
    std::string line;
    while (std::getline(inputStream, line)) {
        if (DEBUG) Util::printDebug("[Parser] " + line);

        int fromRow, fromCol, toRow, toCol;
        // some sanity checks
        if (line.length() < 4 || line.length() > 5 || line[0] < 'a' || line[0] > 'h' || line[1] < '0' || line[1] > '9'
            || line[2] < 'a' || line[2] > 'h' || line[3] < '0' || line[3] > '9')
            Util::exitError("[Parser] Failed sanity checks for line " + line);

        std::string from = line.substr(0, 2);
        std::string to = line.substr(2, 2);

        // check that parsing is correct
        int from_square = coord_to_sq(from);
        int to_square = coord_to_sq(to);
        char promotion_piece = (line.length() == 5) ? line[4] : ' ';

        if (DEBUG) std::cout << "[Parser] " << from_square << "->" << to_square << std::endl;
        if (DEBUG)
            std::cout << "[Parser] " << sq_to_coord(from_square) << "->" << sq_to_coord(to_square) << "==>"
                      << promotion_piece << std::endl;

        BitMove line_move = parse_algebraic_move(from_square, to_square, promotion_piece);
        board.move(line_move, false);

        if (DEBUG) std::cout << "ep::" << board.enpassantSquare << ", prom_piece:" << promotion_piece << std::endl;
        // if (DEBUG) std::cout << board;
    }

    //NICEDEBUG
    // Util::printDebug(std::string("[Parser] ") + "---------End parsing board state:");
    // std::cout << board;
    // Util::printDebug(std::string("[Parser] ") + "End parsing board state-------------");
    std::cout << "[Parser][ply=] " << board.ply << std::endl;
    inputStream.close();
}

// FIXME: BUG: fix this parsing !
BitMove Parser::parse_algebraic_move(int from, int to, char promotion_code) {
    int piece = board.get_piece_on_square(from);
    bool capture = false;
    bool doublepush = false;
    bool enpassant = false;
    bool castling = false;
    int prom = NO_PROMOTION;

    if (piece == -1)
        Util::exitError("[parse_algebraic_move] ERROR, HISTORY INVALID, NOT HANDLED, SHOULD NEVER HAPPEN...PANIC!!");

    if (board.get_piece_on_square(to) != -1) capture = true;

    if (promotion_code != ' ') {
        prom = char_to_pieces(promotion_code);
    };

    const int deltaSquareAbs = std::abs(to - from);

    if (piece == PAWN || piece == pawn) {
        // detect pawn doublepush
        if (deltaSquareAbs == 16) doublepush = true;
        // detect pawn enpassant opportunity
        if (to % 8 != from % 8 && board.get_piece_on_square(to) == -1) enpassant = true;
    }

    if (piece == KING || piece == king)
        if (deltaSquareAbs == 2) castling = true;

    // FIXME: temporary !!!
    int promotionPiece = ((promotion_code == 'k') ? char_to_pieces('n') : char_to_pieces(promotion_code));
    if (DEBUG) {
        std::cout << "got parse promcode : " << promotion_code
                  << " which corresponds to piece : " << letter_pieces[promotionPiece] << std::endl;
    }
    return BitMove(from, to, piece, promotionPiece, capture, doublepush, enpassant, castling);
};

void Parser::writeNextMove(const std::string &moveString) {
    std::ofstream outputStream(outputFile, std::ios::trunc);
    if (!outputStream) Util::exitError("[Parser] Could not open file `" + outputFile + "` for writing.");

    outputStream << moveString << std::endl;
    Util::printDebug("[Parser] Wrote to ouput file " + outputFile + " move : " + moveString);

    outputStream.close();
}