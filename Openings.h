#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Board.h"

//we will store openings objects of this class
class Openings {
   private:
    //map of openings
    std::unordered_map<std::string, std::vector<std::string>> openings; //map : position -> best moves

   public:
    // constructor
    Openings(const std::string& filePath);

    //load openings from a file
    void loadOpenings(const std::string& filePath);

    //get a move from a fen
    std::string getMove(Board &board);
};