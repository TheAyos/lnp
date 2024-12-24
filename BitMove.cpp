#include "BitMove.h"

#include <iostream>
#include <string>
#include <vector>

/* --------------------------------- BitMove -------------------------------- */

BitMove::BitMove(
    int from, int to, int piece, int promotion, bool capture, bool doublepush, bool enpassant, bool castling) {
    m_bit = (from)  // 6 bits each..
            | (to << 6) | (piece << 12)  // 4 bits each..
            | (promotion << 16) | (capture << 20)  // 1 bit each..
            | (doublepush << 21) | (enpassant << 22) | (castling << 23);
};

// OPTI inline ? static ?

int BitMove::get_from() const {
    return m_bit & 0b111111;
};
int BitMove::get_to() const {
    return (m_bit >> 6) & 0b111111;
};

int BitMove::get_piece() const {
    return (m_bit >> 12) & 0b1111;
};
int BitMove::get_promotion_piece() const {
    return (m_bit >> 16) & 0b1111;
};

int BitMove::get_capture() const {
    return (m_bit >> 20) & 0b1;
};
int BitMove::get_doublepush() const {
    return (m_bit >> 21) & 0b1;
};
int BitMove::get_enpassant() const {
    return (m_bit >> 22) & 0b1;
};
int BitMove::get_castling() const {
    return (m_bit >> 23) & 0b1;
};

char get_promoted_letter(int promoted) {
    if (promoted == PAWN || promoted == pawn || promoted == KING || promoted == king)
        return ' ';
    return letter_pieces[promoted];
}

void BitMove::print() {
    std::cout << letter_pieces[get_piece()] << ":" << sq_to_coord(get_from()) << "->" << sq_to_coord(get_to())
              << ", capflag: " << get_capture() << ", "
              << ", promoted: " << get_promoted_letter(get_promotion_piece()) << ", "
              << "doublepush: " << get_doublepush() << ", "
              << "enpassant: " << get_enpassant() << ", "
              << "castling: " << get_castling() << std::endl;
}

std::string BitMove::get_algebraic_notation() {
    if (get_promotion_piece())
        return std::string(sq_to_coord(get_from())) + sq_to_coord(get_to()) + get_promoted_letter(get_promotion_piece());
    else
        return std::string(sq_to_coord(get_from())) + sq_to_coord(get_to());
}

/* ------------------------------- BitMoveVec ------------------------------- */

BitMoveVec::BitMoveVec() : std::vector<BitMove>(){};

std::ostream& operator<<(std::ostream& os, const BitMoveVec& moves) {
    if (moves.empty()) {
        std::cout << "\n     No move in the move list!\n";
        return os;
    }

    os << "\n     move    piece     capture   double    enpass    castling\n\n";

    for (const BitMove& move : moves) {
        os << "     " << sq_to_coord(move.get_from()) << sq_to_coord(move.get_to())
           << (move.get_promotion_piece() ? get_promoted_letter(move.get_promotion_piece()) : ' ') << "   "
           << letter_pieces[move.get_piece()] << "         " << (move.get_capture() ? 1 : 0) << "         "
           << (move.get_doublepush() ? 1 : 0) << "         " << (move.get_enpassant() ? 1 : 0) << "         "
           << (move.get_castling() ? 1 : 0) << "\n";
    }
    os << "\n\n     Total number of moves: " << moves.size() << "\n\n";
    return os;
}