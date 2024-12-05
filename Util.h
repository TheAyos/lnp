#pragma once
#include <string>

namespace Util {
  extern bool debugMode;
  void printError(const std::string &message);
  void printDebug(const std::string &message);
} // namespace Util