#include "Parser.h"
#include "Util.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

// TODO: maybe add tests later ?

Parser::Parser(int argc, char **argv) : argc{argc}, argv{argv} {
}

int Parser::parseArgs() {
  for(int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if(arg != "-H" && arg != "-m")
      Util::exitError("Unrecognized argument `" + arg + "`.");

    if(i + 1 < argc && argv[i + 1][0] != '-')
      argMap[arg] = argv[++i]; // store next argument
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
}

int Parser::parseHistory() {

  std::ifstream inputStream(historyFile);

  if(!inputStream || !inputStream.is_open())
    Util::exitError("Could not open file `" + historyFile + "`.");

  std::string line;
  while(std::getline(inputStream, line)) {
    std::cout << line << std::endl;
  }

  inputStream.close();
  return 0;
}

int Parser::writeNextMove(const std::string &moveString) {
  std::ofstream outputStream(outputFile, std::ios::trunc);
  if(!outputStream)
    Util::exitError("Could not open file `" + outputFile + "` for writing.");

  outputStream << moveString << std::endl;
  Util::printDebug("Wrote to ouput file " + outputFile
                   + " move : " + moveString);

  outputStream.close();
  return 0;
}
