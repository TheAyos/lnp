#include "Parser.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Board.h"

Parser::Parser(int argc, char **argv) : argc{argc}, argv{argv} {
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

void Parser::parseHistory(Board &board) {
    std::ifstream inputStream(historyFile);

    if (!inputStream || !inputStream.is_open()) Util::exitError("Could not open file `" + historyFile + "`.");
    std::string line;
    while (std::getline(inputStream, line)) {
        Util::printDebug("[Parser] " + line);

        int fromRow, fromCol, toRow, toCol;
        // some sanity checks
        if (line.length() < 4 || line.length() > 5 || line[0] < 'a' || line[0] > 'h' || line[1] < '0' || line[1] > '9'
            || line[2] < 'a' || line[2] > 'h' || line[3] < '0' || line[3] > '9')
            Util::exitError("[Parser] Failed sanity checks for line " + line);

        std::string from = line.substr(0, 2);
        std::string to = line.substr(2, 4);

        // check that parsing is correct
        std::cout << "[Parser] " << coord_to_sq(from) << "->" << coord_to_sq(to) << std::endl;
        std::cout << "[Parser] " << sq_to_coord(coord_to_sq(from)) << "->" << sq_to_coord(coord_to_sq(to)) << std::endl;

        int from_square = coord_to_sq(from);
        int to_square = coord_to_sq(to);
        char promotion_piece = (line.length() == 5) ? line[4] : ' ';

        BitMove line_move = board.parse_algebraic_move(from_square, to_square, promotion_piece);
        board.move(line_move);

        // std::cout << board;
    }

    inputStream.close();
}

void Parser::writeNextMove(const std::string &moveString) {
    std::ofstream outputStream(outputFile, std::ios::trunc);
    if (!outputStream) Util::exitError("[Parser] Could not open file `" + outputFile + "` for writing.");

    outputStream << moveString << std::endl;
    Util::printDebug("[Parser] Wrote to ouput file " + outputFile + " move : " + moveString);

    outputStream.close();
}