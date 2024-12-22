#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "BitBoard.h"
#include "Util.h"

struct Parser {
    int argc;
    char **argv;
    std::unordered_map<std::string, std::string> argMap;

    std::string historyFile;
    std::string outputFile;

    Parser(int argc, char **argv);
    int parseArgs();
    int parseHistory(Board &board);
    int writeNextMove(const std::string &moveString);
};

Parser::Parser(int argc, char **argv) : argc{argc}, argv{argv} {
}

int Parser::parseArgs() {
    // for each argument
    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if(arg != "-H" && arg != "-m")
            Util::exitError("Unrecognized argument `" + arg + "`.");

        if(i + 1 < argc && argv[i + 1][0] != '-')
            argMap[arg] = argv[++i];  // store next argument
        else
            Util::exitError("Missing value for option `" + arg + "`.");
    }

    if(argMap.count("-H")) {
        historyFile = argMap["-H"];
        Util::printDebug("Input history filename: " + historyFile);
    } else {
        Util::exitError("-H option is required.");
    }

    if(argMap.count("-m")) {
        outputFile = argMap["-m"];
        Util::printDebug("Output move file: " + outputFile);
    } else {
        Util::exitError("-m option is required.");
    }
    return 0;
}

int Parser::parseHistory(Board &board) {
    int turn = 1;
    std::ifstream inputStream(historyFile);

    if(!inputStream || !inputStream.is_open())
        Util::exitError("Could not open file `" + historyFile + "`.");
    std::string line;
    while(std::getline(inputStream, line)) {
        std::cout << line << std::endl;

        int fromRow, fromCol, toRow, toCol;
        // some sanity checks
        if(line.length() < 4 || line.length() > 5 || line[0] < 'a' || line[0] > 'h' || line[1] < '0' || line[1] > '9'
           || line[2] < 'a' || line[2] > 'h' || line[3] < '0' || line[3] > '9')
            Util::exitError("Failed sanity checks for line " + line);

        std::string from = line.substr(0, 2);
        std::string to = line.substr(2, 4);

        // check that parsing is correct
        std::cout << coord_to_sq[from] << "->" << coord_to_sq[to] << std::endl;
        std::cout << sq_to_coord[coord_to_sq[from]] << "->" << sq_to_coord[coord_to_sq[to]] << std::endl;

        char promotion_piece = (line.length() == 5) ? line[4] : ' ';

        int from_square = coord_to_sq.at(from);
        int to_square = coord_to_sq.at(to);
        BitMove line_move = BitMove(from_square, to_square, NOPIECE, char_to_pieces[promotion_piece], 0, 0, 0, 0);
        board.move(line_move);

        std::cout << board;
        turn = 1 - turn;  // switch turns
    }

    inputStream.close();
    return turn;
}

int Parser::writeNextMove(const std::string &moveString) {
    std::ofstream outputStream(outputFile, std::ios::trunc);
    if(!outputStream)
        Util::exitError("Could not open file `" + outputFile + "` for writing.");

    outputStream << moveString << std::endl;
    Util::printDebug("Wrote to ouput file " + outputFile + " move : " + moveString);

    outputStream.close();
    return 0;
}