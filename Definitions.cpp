#include "Definitions.h"

#include <iostream>
#include <string>

// clang-format off

//FIXME: crucial: rewrite these conversion functions efficiently

// FIXME: rewrite using the same convention as teacher
int char_to_pieces(char c) {
    std::map<char, int> _char_to_pieces
        = {{'P', PAWN}, {'N', KNIGHT}, {'B', BISHOP}, {'R', ROOK}, {'Q', QUEEN}, {'K', KING},
           {'p', pawn}, {'n', knight}, {'b', bishop}, {'r', rook}, {'q', queen}, {'k', king}};
    return _char_to_pieces[c];
}

char promoted_pieces(int c) {
    std::map<int, char> _promoted_pieces = {{QUEEN, 'q'}, {ROOK, 'r'}, {BISHOP, 'b'}, {KNIGHT, 'n'},
                                            {queen, 'q'}, {rook, 'r'}, {bishop, 'b'}, {knight, 'n'}};
    return _promoted_pieces[c];
}


// regex ([abcdefgh][1-8]), // "$1",
// inline const char* sq_to_coord(int sq) {
//     return _sq_to_coord[sq];
// }
// int coord_to_sq(const std::string& coord) {
//     std::map<std::string, int> _coord_to_sq = {
//         {"a8", a8}, {"b8", b8}, {"c8", c8}, {"d8", d8}, {"e8", e8}, {"f8", f8}, {"g8", g8}, {"h8", h8},
//         {"a7", a7}, {"b7", b7}, {"c7", c7}, {"d7", d7}, {"e7", e7}, {"f7", f7}, {"g7", g7}, {"h7", h7},
//         {"a6", a6}, {"b6", b6}, {"c6", c6}, {"d6", d6}, {"e6", e6}, {"f6", f6}, {"g6", g6}, {"h6", h6},
//         {"a5", a5}, {"b5", b5}, {"c5", c5}, {"d5", d5}, {"e5", e5}, {"f5", f5}, {"g5", g5}, {"h5", h5},
//         {"a4", a4}, {"b4", b4}, {"c4", c4}, {"d4", d4}, {"e4", e4}, {"f4", f4}, {"g4", g4}, {"h4", h4},
//         {"a3", a3}, {"b3", b3}, {"c3", c3}, {"d3", d3}, {"e3", e3}, {"f3", f3}, {"g3", g3}, {"h3", h3},
//         {"a2", a2}, {"b2", b2}, {"c2", c2}, {"d2", d2}, {"e2", e2}, {"f2", f2}, {"g2", g2}, {"h2", h2},
//         {"a1", a1}, {"b1", b1}, {"c1", c1}, {"d1", d1}, {"e1", e1}, {"f1", f1}, {"g1", g1}, {"h1", h1}
//     };
//     return _coord_to_sq.at(coord);
// }

int coord_to_sq(const std::string& coord) {
    if (coord.length() != 2 || coord[0] < 'a' || coord[0] > 'h' || coord[1] < '1' || coord[1] > '8') {
        throw std::invalid_argument("wtf invalid coord");
    }
    int file = coord[0] - 'a';
    int rank = '8' - coord[1];
    return rank * 8 + file;
}




namespace Util {
    bool debugMode = true;
    void exitError(const std::string &message) {
        std::cerr << "Error: " << message << std::endl;
        exit(1);
    }

    void printDebug(const std::string &message) {
        if (debugMode) std::cout << "[DEBUG] " << message << std::endl;
    }
}  // namespace Util
