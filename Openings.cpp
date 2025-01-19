#include "Openings.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

Openings::Openings(const std::string &filePath) {
    srand(time(0)); // seed the random number generator
    loadOpenings(filePath);
}

/*
We want to read a file where eahc line is :
"FEN + " " + list of moves separated by a space"
ex : rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1 e7e5 c7c5 g8f6

We are using a map so :
    - the FEN will be used as a key
    - the list of moves will be the values
*/

/*
_________________________________________________________________
            Source of the openings database 

_________________________________________________________________

https://github.com/tomgp/chess-canvas/blob/master/pgn/chess_openings.csv
*/




/*
_____________________________________________________________
            Load from the opening file
_____________________________________________________________
*/
void Openings::loadOpenings(const std::string& filePath) {
    std::ifstream file(filePath); // ifstream -> read a file

    //manage error if we can't load the file
    if (!file.is_open()) {
        std::cerr << "Error : can't load the file " << filePath << std::endl;
        return;
    }

    //each line of the file
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line); //strings as a stream
        std::string fen, move, nothing;
        std::vector<std::string> moves;

        //check there is a fen
        std::string part;
        for (int i = 0; i < 3; i++) {
            if (!(iss >> part)) {
                continue; //invalid line
            }
            fen += part + " ";
        }

        std::string partn;
        for (int i = 0; i < 3; i++) {
            if (!(iss >> partn)) {
                continue; //invalid line
            }
            nothing += partn + " ";
        }

        
        
        while (iss >> move) { // >> allows us to read all moves
            moves.push_back(move);
        }
        openings[fen] = moves; // store the opening

    }
    file.close();
    // // debug print the contents of the openings map
    // for (const auto& entry : openings) {
    //     std::cout << "FEN: " << entry.first << " Moves: ";
    //     for (const auto& move : entry.second) {
    //         std::cout << move << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

/*
_____________________________________________________________
            Get moves for a particular opening
_____________________________________________________________
*/
std::string Openings::getMove(Board &board) {
    std::string fen = board.getFEN() + " ";

    auto key_fen = openings.find(fen);
    if (key_fen != openings.end() && !key_fen->second.empty()) { // second gives us the value of a key
        return key_fen->second[rand() % key_fen->second.size()]; //take a move randomly
    }
    return ""; //no move found
}
