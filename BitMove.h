#pragma once
#include "BitBoard.h"

//FIXME: (would uint32_t be better ? speed ?)
/*  move template (ULL is 64 bits)
    this encodes relevant move information in a single 64 bit integer
    0000 0000 0000 0000 0000 0000 0011 1111   from square
    0000 0000 0000 0000 0000 1111 1100 0000   to square
    0000 0000 0000 0000 1111 0000 0000 0000   piece code
    0000 0000 0000 1111 0000 0000 0000 0000   promoted piece code
    0000 0000 0001 0000 0000 0000 0000 0000   is capture ?
    0000 0000 0010 0000 0000 0000 0000 0000   is double pawn push ?
    0000 0000 0100 0000 0000 0000 0000 0000   is enpassant ?
    0000 0000 1000 0000 0000 0000 0000 0000   is castling ?
    N.B: still have 40 free bits to use
*/
class BitMove {
    U64 bit;
    public:
        BitMove() : bit(0ULL) {};
        BitMove(int from, int to, int promotion, int capture, int doublepush, int enpassant, int castling, int piece) {
        bit =
            // 6 bits each
            (from) 
            | (to << 6)
            // 4 bits each
            | (piece << 12) 
            | (promotion << 16)
            // 1 bit each
            | (capture << 20) 
            | (doublepush << 21) 
            | (enpassant << 22) 
            | (castling << 23);
    };
        int encode();
        int get_from() { return bit & 0b111111; };
        int get_to() { return (bit >> 6) & 0b111111; };

        int get_piece() { return (bit >> 12) & 0b1111; };
        int get_promoted() { return (bit >> 16) & 0b1111; };

        int get_capture() { return (bit >> 20) & 0b1; };
        int get_doublepush() { return (bit >> 21) & 0b1; };
        int get_enpassant() { return (bit >> 22) & 0b1; };
        int get_castling() { return (bit >> 23) & 0b1; };

        // void set_from(int from);
        // void set_to(int to);

        // void set_piece(int piece);
        // void set_promoted(int promotion);

        // void set_capture(int capture);
        // void set_doublepush(int doublepush);
        // void set_enpassant(int enpassant);
        // void set_castling(int castling);

        void print();
        std::string get_algebraic_notation();
};

BitMove::BitMove(int from, int to, int promotion, int capture, int doublepush, int enpassant, int castling, int piece) {
    bit = (from) 
        | (to << 6)
        | (piece << 12) 
        | (promotion << 16)
        | (capture << 20) 
        | (doublepush << 21) 
        | (enpassant << 22) 
        | (castling << 23);
};

char get_promoted_letter(int promoted) {
    if (promoted == PAWN || promoted == pawn || promoted == KING || promoted == king) return ' ';
    return letter_pieces[promoted];
}

void BitMove::print() {
    std::cout << letter_pieces[get_piece()] << ":" 
              << sq_to_coord[get_from()] << "->"
              << sq_to_coord[get_to()]
              << ", capflag: " << get_capture() << ", "
              << ", promoted: " << get_promoted_letter(get_promoted()) << ", "
              << "doublepush: " << get_doublepush() << ", "
              << "enpassant: " << get_enpassant() << ", "
              << "castling: " << get_castling() << std::endl;
}

std::string BitMove::get_algebraic_notation() {
    char prom = get_promoted_letter(get_promoted());
    std::string move = std::string(sq_to_coord[get_from()]) + std::string(sq_to_coord[get_to()]);
    return (prom != ' ') ? move + prom : move;
}