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

const std::string FEN_POS_STARTING = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string FEN_POS_2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
const std::string FEN_POS_3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
const std::string FEN_POS_4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
const std::string FEN_POS_4b = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ";
