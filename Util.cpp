#include "Util.h"
#include <iostream>

namespace Util {
  bool debugMode = true;

  void printError(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
    exit(1);
  }

  void printDebug(const std::string &message) {
    if(debugMode)
      std::cout << "[DEBUG] " << message << std::endl;
  }
} // namespace Util