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

    int fromRow, fromCol, toRow, toCol;
    // some sanity checks
    if(line.length() < 5 || line.length() > 6 || line[0] < 'a' || line[0] > 'h'
       || line[1] < '0' || line[1] > '9' || line[2] < 'a' || line[2] > 'h'
       || line[3] < '0' || line[3] > '9')
      return -1;

    fromCol = (line[0] - 'a'); // ascii magic
    fromRow = std::stoi(line.substr(1, 1));
    toCol = (line[2] - 'a');
    toRow = std::stoi(line.substr(3, 1));

    Util::printDebug(std::to_string(fromRow) + " " + std::to_string(fromCol)
                     + "->" + std::to_string(toRow) + " "
                     + std::to_string(toCol));

    if(line.length() == 6) {
      std::string newPiece(1, line[4]);
      Util::printDebug("With promotion from ?? to " + newPiece);
    }

    // TODO: castling detection, when Board representation is done (castling
    // rights & states)
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
