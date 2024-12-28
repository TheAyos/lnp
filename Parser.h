#pragma once
#include <string>
#include <unordered_map>

#include "BitMove.h"
#include "Definitions.h"

class Board;
class Parser {
   public:
    Board &board;
    int argc;
    char **argv;

    Parser(Board &board, int argc, char **argv);

    void parseArgs();

    void parseHistory();

    // construct valid BitMove object from a move in algebraic notation (e.g. e2e4, e7e8q) from parser input
    BitMove parse_algebraic_move(int from, int to, char promotion_piece);

    void writeNextMove(const std::string &moveString);

   private:
    std::unordered_map<std::string, std::string> argMap;
    std::string historyFile;
    std::string outputFile;
};