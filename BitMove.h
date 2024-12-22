#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "BitBoard.h"
#include "Definitions.h"


// FIXME: (would uint32_t be better ? speed ?)
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
    U64 m_bit = 0ULL;

   public:
    BitMove(int from, int to, int piece, int promotion, bool capture, bool doublepush, bool enpassant, bool castling) {
        m_bit = (from)  // 6 bits each..
                | (to << 6) | (piece << 12)  // 4 bits each..
                | (promotion << 16) | (capture << 20)  // 1 bit each..
                | (doublepush << 21) | (enpassant << 22) | (castling << 23);
    };

    //OPTI inline ? static ?
    
    int get_from() const {
        return m_bit & 0b111111;
    };
    int get_to() const {
        return (m_bit >> 6) & 0b111111;
    };

    int get_piece() const {
        return (m_bit >> 12) & 0b1111;
    };
    int get_promoted() const {
        return (m_bit >> 16) & 0b1111;
    };

    int get_capture() const {
        return (m_bit >> 20) & 0b1;
    };
    int get_doublepush() const {
        return (m_bit >> 21) & 0b1;
    };
    int get_enpassant() const {
        return (m_bit >> 22) & 0b1;
    };
    int get_castling() const {
        return (m_bit >> 23) & 0b1;
    };

    void print();
    std::string get_algebraic_notation();
};

char get_promoted_letter(int promoted) {
    if(promoted == PAWN || promoted == pawn || promoted == KING || promoted == king)
        return ' ';
    return letter_pieces[promoted];
}

void BitMove::print() {
    std::cout << letter_pieces[get_piece()] << ":" << sq_to_coord[get_from()] << "->" << sq_to_coord[get_to()]
              << ", capflag: " << get_capture() << ", "
              << ", promoted: " << get_promoted_letter(get_promoted()) << ", "
              << "doublepush: " << get_doublepush() << ", "
              << "enpassant: " << get_enpassant() << ", "
              << "castling: " << get_castling() << std::endl;
}

std::string BitMove::get_algebraic_notation() {
    if(get_promoted())
        return std::string(sq_to_coord[get_from()]) + sq_to_coord[get_to()] + get_promoted_letter(get_promoted());
    else
        return std::string(sq_to_coord[get_from()]) + sq_to_coord[get_to()];
}

class BitMoveVec : public std::vector<BitMove> {
   public:
    BitMoveVec() : std::vector<BitMove>() {};

    friend std::ostream& operator<<(std::ostream& os, const BitMoveVec& moves);
};

std::ostream& operator<<(std::ostream& os, const BitMoveVec& moves) {
    if(moves.empty()) {
        std::cout << "\n     No move in the move list!\n";
        return os;
    }

    os << "\n     move    piece     capture   double    enpass    castling\n\n";

    for(const BitMove& move : moves) {
        os << "     " << sq_to_coord[move.get_from()] << sq_to_coord[move.get_to()]
                  << (move.get_promoted() ? get_promoted_letter(move.get_promoted()) : ' ') << "   "
                  << letter_pieces[move.get_piece()] << "         " << (move.get_capture() ? 1 : 0) << "         "
                  << (move.get_doublepush() ? 1 : 0) << "         " << (move.get_enpassant() ? 1 : 0) << "         "
                  << (move.get_castling() ? 1 : 0) << "\n";
    }
    os << "\n\n     Total number of moves: " << moves.size() << "\n\n";
    return os;
}