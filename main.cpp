#include <iostream>
#include <string>
#include <unordered_map>

/*
How to open a file for reading :
std::ifstream inputFile("input.txt");

How to open a file for writing :
std::ofstream outputFile("output.txt");

Read a line from the file :
std::getline(inputFile, line)
--> while (std::getline(inputFile, line))
it reads each line from the file and stores it in the line string, it continues
until the end of the file is reached
*/

int main(int argc, char **argv) {

  for(int i = 0; i < argc; i++) {
    std::cout << "Displaying argument #" << std::to_string(i) << ": `"
              << std::string(argv[i]) << "'" << std::endl;
  }

  std::unordered_map<std::string, std::string> argMap;

  for(int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if(arg == "-H" || arg == "-m") {
      if(i + 1 < argc) {
        argMap[arg] = argv[++i]; // store next argument
      } else {
        std::cerr << "Error: Missing value for option `" << arg << "`."
                  << std::endl;
        return 1;
      }
    } else {
      std::cerr << "Error: Unrecognized argument `" << arg << "`." << std::endl;
      return 1;
    }
  }
  // <yourAIname> -H <input history file> -m <output move file>

  if(argMap.count("-H")) {
    std::cout << "Input history filename: " << argMap["-H"] << std::endl;
  } else {
    std::cerr << "Error: -H option is required." << std::endl;
    return 1;
  }

  if(argMap.count("-m")) {
    std::cout << "Output move file: " << argMap["-m"] << std::endl;
  } else {
    std::cerr << "Error: -m option is required." << std::endl;
    return 1;
  }

  return 0;
}