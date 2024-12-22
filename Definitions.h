#pragma once
#include <map>

using U64 = unsigned long long;

/* -------------------------------------------------------------------------- */
/*                        Board representation related                        */
/* -------------------------------------------------------------------------- */
enum Files { a, b, c, d, e, f, g, h };

enum Pieces { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, pawn, knight, bishop, rook, queen, king, NOPIECE };

enum Colors { W, B, WB };

std::map<char, int> char_to_pieces
    = {{'P', PAWN}, {'N', KNIGHT}, {'B', BISHOP}, {'R', ROOK}, {'Q', QUEEN}, {'K', KING},
       {'p', pawn}, {'n', knight}, {'b', bishop}, {'r', rook}, {'q', queen}, {'k', king}};

const char letter_pieces[] = "PNBRQKpnbrqk";

std::map<int, char> promoted_pieces = {
    {QUEEN, 'q'}, {ROOK, 'r'}, {BISHOP, 'b'}, {KNIGHT, 'n'}, {queen, 'q'}, {rook, 'r'}, {bishop, 'b'}, {knight, 'n'}};

// clang-format off

// square enumeration, a8 -> LSB (0), h1 -> MSB (63)
// https://wiki.sharewiz.net/doku.php?id=chess:programming:bitboards
enum Squares {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
};

// regex ([abcdefgh][1-8]), // "$1",
const char* sq_to_coord[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};

std::map<std::string, int> coord_to_sq = {
    {"a8", a8}, {"b8", b8}, {"c8", c8}, {"d8", d8}, {"e8", e8}, {"f8", f8}, {"g8", g8}, {"h8", h8},
    {"a7", a7}, {"b7", b7}, {"c7", c7}, {"d7", d7}, {"e7", e7}, {"f7", f7}, {"g7", g7}, {"h7", h7},
    {"a6", a6}, {"b6", b6}, {"c6", c6}, {"d6", d6}, {"e6", e6}, {"f6", f6}, {"g6", g6}, {"h6", h6},
    {"a5", a5}, {"b5", b5}, {"c5", c5}, {"d5", d5}, {"e5", e5}, {"f5", f5}, {"g5", g5}, {"h5", h5},
    {"a4", a4}, {"b4", b4}, {"c4", c4}, {"d4", d4}, {"e4", e4}, {"f4", f4}, {"g4", g4}, {"h4", h4},
    {"a3", a3}, {"b3", b3}, {"c3", c3}, {"d3", d3}, {"e3", e3}, {"f3", f3}, {"g3", g3}, {"h3", h3},
    {"a2", a2}, {"b2", b2}, {"c2", c2}, {"d2", d2}, {"e2", e2}, {"f2", f2}, {"g2", g2}, {"h2", h2},
    {"a1", a1}, {"b1", b1}, {"c1", c1}, {"d1", d1}, {"e1", e1}, {"f1", f1}, {"g1", g1}, {"h1", h1}
};

// const int coord_to_sq[] = {
//     a8, b8, c8, d8, e8, f8, g8, h8,
//     a7, b7, c7, d7, e7, f7, g7, h7,
//     a6, b6, c6, d6, e6, f6, g6, h6,
//     a5, b5, c5, d5, e5, f5, g5, h5,
//     a4, b4, c4, d4, e4, f4, g4, h4,
//     a3, b3, c3, d3, e3, f3, g3, h3,
//     a2, b2, c2, d2, e2, f2, g2, h2,
//     a1, b1, c1, d1, e1, f1, g1, h1,
// };

// clang-format on

/* -------------------------------------------------------------------------- */
/*                          used by attack generation                         */
/* -------------------------------------------------------------------------- */
const U64 not_file_masks[8] = {
    18374403900871474942ULL, 18302063728033398269ULL, 18157383382357244923ULL, 17868022691004938231ULL,
    17289301308300324847ULL, 16131858542891098079ULL, 13816973012072644543ULL, 9187201950435737471ULL,
};

const U64 not_gh_file = not_file_masks[h] & not_file_masks[g];
const U64 not_ab_file = not_file_masks[a] & not_file_masks[b];

/* castling rights bit template (takes 4 bits)
    - 0001 : white kingside
    - 0010 : white queenside
    - 0100 : black kingside
    - 1000 : black queenside
    ...
    - 0000 : no castling
    - 1111 : all castling rights
 */
enum CastlingRights { WK = 1, WQ = 2, BK = 4, BQ = 8 };