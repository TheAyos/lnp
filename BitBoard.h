#include <iostream>
typedef unsigned long long U64;

/* source : https://wiki.sharewiz.net/doku.php?id=chess:programming:bitboards
 RANKS:
   8 |       56   57   58   59   60   61   62   63  (MSB,
   7 |       48   49   50   51   52   53   54   55  left)
   6 |       40   41   42   43   44   45   46   47
   5 |       32   33   34   35   36   37   38   39
   4 |       24   25   26   27   28   29   30   31
   3 |       16   17   18   19   20   21   22   23
   2 |        8    9   10   11   12   13   14   15
   1 | (LSB,  0    1    2    3    4    5    6    7
       right)
           -------------------------------------------
 FILES:      a     b    c    d    e    f    g    h
 */

// clang-format off
enum Files { a, b, c, d, e, f, g, h };
// square enumeration, from a1 to h8 (0 to 63)
enum Squares {
    a8,b8,c8,d8,e8,f8,g8,h8,
    a7,b7,c7,d7,e7,f7,g7,h7,
    a6,b6,c6,d6,e6,f6,g6,h6,
    a5,b5,c5,d5,e5,f5,g5,h5,
    a4,b4,c4,d4,e4,f4,g4,h4,
    a3,b3,c3,d3,e3,f3,g3,h3,
    a2,b2,c2,d2,e2,f2,g2,h2,
    a1,b1,c1,d1,e1,f1,g1,h1,
};

enum Colors { white, black };
// clang-format o

// struct bitboard_t {
//     U64 bitboard = 0ULL;
//     // U64 whiteKing;
//     // U64 whiteQueens;
//     // U64 whiteRooks;
//     // U64 whiteBishops;
//     // U64 whiteKnights;
//     // U64 whitePawns;
//     // U64 blackKing;
//     // U64 blackQueens;
//     // U64 blackRooks;
//     // U64 blackBishops;
//     // U64 blackKnights;
//     // U64 blackPawns;
//     // U64 whitePieces;
//     // U64 blackPieces;
//     bitboard_t();
//     bitboard_t(U64 bitboard);
//     // void display();
//     // U64 get_bit(int square);
//     // void set_bit(int square);
//     // void clear_bit(int square);
// };
// typedef struct bitboard_t BB;
// bitboard_t::bitboard_t() : bitboard(4ULL) {
//     set_bit(e2);
//     display();
// }
// bitboard_t::bitboard_t(U64 bitboard) : bitboard(bitboard) {
// }

/* ----------------------------- bit operations ----------------------------- */

U64 get_bit(const U64& bitboard, int square) {
    return (bitboard >> square) & 1ULL;
}
void set_bit(U64& bitboard, int square) {
    bitboard |= (1ULL << square);
}
void clear_bit(U64& bitboard, int square) {
    bitboard &= ~(1ULL << square);
}

int rf_to_square(int rank, int file) {
    return 8 * rank + file;
}
int square_to_rf(int square) {
    return square % 8, square / 8;
}

void display_bits(const U64& bitboard){
    for (int rank = 0; rank < 8; rank++){
        std::cout << std::to_string(8 - rank) << "   ";
        for (int file = 0; file < 8; file++){
            int square = 8 * rank + file;
            std::cout << " " << get_bit(bitboard, square);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "    ";
    for (int file = 0; file < 8; file++){
        std::cout << " " << (char)('a' + file);
    }
    std::cout << std::endl;
    std::cout << "  BB repr: " << bitboard << std::endl;
};


void display(const U64& bitboard) {
    const std::string footer = "    a    b    c    d    e    f    g    h";
    const std::string emptySquare = "   ";

    std::string line = "  ";
    for(int i = 0; i < 8; i++) 
        line += " ----";
    line += "\n";

    std::string output = line;

    for (int rank = 0; rank < 8; ++rank) {
        output += std::to_string(8 - rank) + " ";
        for (int file = 0; file < 8; ++file) {
            output += "| ";
            int square = rf_to_square(rank, file);
            std::string pieceRepr = get_bit(bitboard, square) ? " x " : "   ";
            output += get_bit(bitboard, square) ? pieceRepr : emptySquare;
        }
        output += "|\n" + line;
    }

    output += footer;
    std::cout << output << std::endl;
    std::cout << "  BB repr: " << bitboard << std::endl;
}
