#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

struct Parser {
  int argc;
  char **argv;
  std::unordered_map<std::string, std::string> argMap;

  std::string historyFile;
  std::string outputFile;

  Parser(int argc, char **argv);
  int parseArgs();
  int parseHistory();
  int writeNextMove(const std::string &moveString);
};
