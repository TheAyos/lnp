#pragma once
#include <map>
#include <string>

int char_to_pieces(char c);
char promoted_pieces(int c);
const char* sq_to_coord(int sq);
int coord_to_sq(const std::string& coord);

using U64 = unsigned long long;

/* -------------------------------------------------------------------------- */
/*                        Board representation related                        */
/* -------------------------------------------------------------------------- */
enum Files { a, b, c, d, e, f, g, h };

enum Pieces { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, pawn, knight, bishop, rook, queen, king, NOPIECE };

enum Colors { W, B, WB };

const char letter_pieces[] = "PNBRQKpnbrqk";

const int NO_PROMOTION = 0;

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


/* -------------------------------------------------------------------------- */
/*                          used by attack generation                         */
/* -------------------------------------------------------------------------- */
const U64 not_file_masks[8] = {
    18374403900871474942ULL, 18302063728033398269ULL, 18157383382357244923ULL, 17868022691004938231ULL,
    17289301308300324847ULL, 16131858542891098079ULL, 13816973012072644543ULL, 9187201950435737471ULL,
};

const U64 not_gh_file = not_file_masks[h] & not_file_masks[g];
const U64 not_ab_file = not_file_masks[a] & not_file_masks[b];

/* -------------------------------------------------------------------------- */
/*                                move updates                                */
/* -------------------------------------------------------------------------- */

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

const int castlingRightsMasks[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

// TOKEEP:
// precalculation code :
// int castlingRightsMasks[64];
// // init all masks to all bits set
// for (int i = 0; i < 64; ++i) {
//     castlingRightsMasks[i] = 0b1111;
// }
// // update masks for specific squares
// castlingRightsMasks[e1] &= ~WK;  // WK king
// castlingRightsMasks[e1] &= ~WQ;  // WQ king
// castlingRightsMasks[h1] &= ~WK;  // WK rook
// castlingRightsMasks[a1] &= ~WQ;  // WQ rook
// castlingRightsMasks[e8] &= ~BK;  // BK king
// castlingRightsMasks[e8] &= ~BQ;  // BQ king
// castlingRightsMasks[h8] &= ~BK;  // BK rook
// castlingRightsMasks[a8] &= ~BQ;  // BQ rook
// // display
// for (int rank = 0; rank < 8; ++rank) {
//     for (int file = 0; file < 8; ++file) {
//         int square = rf_to_square(rank, file);
//         // std::cout << std::bitset<4>(castlingRightsMasks[square]) << " ";
//         if (file) std::cout << ", ";
//         std::cout << castlingRightsMasks[square];
//     }
//     std::cout << std::endl;
// }

namespace Util {
    void exitError(const std::string& message);
    void printDebug(const std::string& message);
}  // namespace Util