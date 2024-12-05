#include "Parser.h"
#include "Util.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

Parser::Parser(int argc, char **argv) : argc{argc}, argv{argv} {
}

int Parser::parseArgs() {
  for(int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if(arg == "-H" || arg == "-m") {
      if(i + 1 < argc && argv[i + 1][0] != '-') {
        argMap[arg] = argv[++i]; // store next argument
      } else {
        Util::printError("Missing value for option `" + arg + "`.");
        return 1;
      }
    } else {
      Util::printError("Unrecognized argument `" + arg + "`.");
      return 1;
    }
  }

  if(argMap.count("-H")) {
    Util::printDebug("Input history filename: " + argMap["-H"]);
    historyFile = argMap["-H"];
  } else {
    Util::printError("-H option is required.");
    return 1;
  }

  if(argMap.count("-m")) {
    Util::printDebug("Output move file: " + argMap["-m"]);
  } else {
    Util::printError("-m option is required.");
    return 1;
  }
}

int Parser::parseHistory() {
  std::ifstream inputFile(argMap["-H"]);
  if(!inputFile) {
    Util::printError("Could not open file `" + argMap["-H"] + "`.");
    return 1;
  }

  std::string line;
  while(std::getline(inputFile, line)) {
    std::cout << line << std::endl;
  }

  inputFile.close();
}