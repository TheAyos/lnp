#pragma once
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


/* -------------------------------------------------------------------------- */
/*                                    enums                                   */
/* -------------------------------------------------------------------------- */

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

// regex ([abcdefgh][1-8]), // "$1",
const char* sq_to_coord[] = {
    "a8","b8","c8","d8","e8","f8","g8","h8",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a1","b1","c1","d1","e1","f1","g1","h1",
};

const int coord_to_sq[] = {
    a8,b8,c8,d8,e8,f8,g8,h8,
    a7,b7,c7,d7,e7,f7,g7,h7,
    a6,b6,c6,d6,e6,f6,g6,h6,
    a5,b5,c5,d5,e5,f5,g5,h5,
    a4,b4,c4,d4,e4,f4,g4,h4,
    a3,b3,c3,d3,e3,f3,g3,h3,
    a2,b2,c2,d2,e2,f2,g2,h2,
    a1,b1,c1,d1,e1,f1,g1,h1,
};

enum Colors { W, B, WB };

/* castling rights bit template
    - 0001 : white kingside
    - 0010 : white queenside
    - 0100 : black kingside
    - 1000 : black queenside
    ...
    - 0000 : no castling
    - 1111 : all castling rights
 */
enum CastlingRights { WK = 1, WQ = 2, BK = 4, BQ = 8 };

enum Pieces { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, pawn, knight, bishop, rook, queen, king };

const char letter_pieces[] = "PNBRQKpnbrqk";
int char_to_pieces[128];

void init_char_to_pieces() {
    for (int i = 0; i < 128; i++) {
        char_to_pieces[i] = -1;
    }
    char_to_pieces['P'] = PAWN;
    char_to_pieces['N'] = KNIGHT;
    char_to_pieces['B'] = BISHOP;
    char_to_pieces['R'] = ROOK;
    char_to_pieces['Q'] = QUEEN;
    char_to_pieces['K'] = KING;
    char_to_pieces['p'] = pawn;
    char_to_pieces['n'] = knight;
    char_to_pieces['b'] = bishop;
    char_to_pieces['r'] = rook;
    char_to_pieces['q'] = queen;
    char_to_pieces['k'] = king;
}

// clang-format on

/* -------------------------------------------------------------------------- */
/*                          used by attack generation                         */
/* -------------------------------------------------------------------------- */
const U64 not_file_masks[8] = {
    18374403900871474942ULL,
    18302063728033398269ULL,
    18157383382357244923ULL,
    17868022691004938231ULL,
    17289301308300324847ULL,
    16131858542891098079ULL,
    13816973012072644543ULL,
    9187201950435737471ULL,
};

const U64 not_gh_file = not_file_masks[h] & not_file_masks[g];
const U64 not_ab_file = not_file_masks[a] & not_file_masks[b];



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
void square_to_rf(int square, int &rank, int &file) {
    rank = square / 8;
    file = square % 8;
}

void display_bits(const U64& bitboard){
    std::cout << std::endl << "BB repr: " << bitboard << std::endl;
    for (int rank = 0; rank < 8; rank++){
        std::cout << std::to_string(8 - rank) << "  ";
        for (int file = 0; file < 8; file++){
            int square = 8 * rank + file;
            std::cout << " " << get_bit(bitboard, square);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "   ";
    for (int file = 0; file < 8; file++){
        std::cout << " " << (char)('a' + file);
    }
    std::cout << std::endl;
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

//OPTI
int count_bits(U64 bb) {
    int nbits = 0;
    while (bb) {
        nbits++;
        // set LSB to 0
        bb &= bb - 1;
    }
    return nbits;
}
//OPTI
int index_ls1b(U64 bb) {
    if (!bb) return -1; // if bitboard is zero, bad
    return count_bits((bb & -bb) - 1);
}


class Board {
    public:
        U64 bitboards[12];
        U64 occupancies[3];

        int turn = W;
        int enpassant_square = -1;
        int castling_rights = 0;

        Board();
        void print_board();
};
Board::Board() {
    set_bit(bitboards[PAWN], g2);
    set_bit(bitboards[PAWN], h2);
    set_bit(bitboards[PAWN], f2);
    set_bit(bitboards[PAWN], e2);
    set_bit(bitboards[PAWN], d2);
    set_bit(bitboards[PAWN], a2);
    set_bit(bitboards[PAWN], b2);
    set_bit(bitboards[PAWN], c2);

    set_bit(bitboards[pawn], g7);
    set_bit(bitboards[pawn], h7);
    set_bit(bitboards[pawn], f7);
    set_bit(bitboards[pawn], e7);
    set_bit(bitboards[pawn], d7);
    set_bit(bitboards[pawn], a7);
    set_bit(bitboards[pawn], b7);
    set_bit(bitboards[pawn], c7);

    set_bit(bitboards[KNIGHT], b1);
    set_bit(bitboards[KNIGHT], g1);
    set_bit(bitboards[knight], b8);
    set_bit(bitboards[knight], g8);
    
    set_bit(bitboards[BISHOP], c1);
    set_bit(bitboards[BISHOP], f1);
    set_bit(bitboards[bishop], c8);
    set_bit(bitboards[bishop], f8);

    set_bit(bitboards[ROOK], a1);
    set_bit(bitboards[ROOK], h1);
    set_bit(bitboards[rook], a8);
    set_bit(bitboards[rook], h8);

    set_bit(bitboards[QUEEN], d1);
    set_bit(bitboards[KING], e1);

    set_bit(bitboards[queen], d8);
    set_bit(bitboards[king], e8);
}

void Board::print_board() {
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


            if (!file) std::cout << std::to_string(8 - rank) << "  ";

            int piece = -1;

            for (int i = 0; i < 12; i++) {
                if (get_bit(bitboards[i], square)) {
                    piece = i;
                    break;
                }
            }           
            std::string pieceRepr = (piece == -1 ? " " : std::string(1, letter_pieces[piece]));
            output += get_bit(bitboards[piece], square) ? pieceRepr + "  " : emptySquare;
        }
        output += "|\n" + line;
    }

    std::cout << "Turn: " << (!turn ? "White" : "Black") << std::endl;
    std::cout << "Enpassant: " << (enpassant_square != -1 ? sq_to_coord[enpassant_square] : "xx");
    std::cout << "\tCastling: " << (castling_rights & WK ? "WK " : "x")
              << (castling_rights & WQ ? "WQ " : "x")
              << (castling_rights & BK ? "BK " : "x")
              << (castling_rights & BQ ? "BQ " : "x") << std::endl;

    output += footer;
    std::cout << output << std::endl;
}
