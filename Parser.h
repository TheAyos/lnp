#pragma once
#include <string>
#include <unordered_map>
#include "Definitions.h"

class Board;
struct Parser {
    int argc;
    char **argv;
    std::unordered_map<std::string, std::string> argMap;

    std::string historyFile;
    std::string outputFile;

    Parser(int argc, char **argv);
    void parseArgs();
    void parseHistory(Board &board);
    void writeNextMove(const std::string &moveString);
};