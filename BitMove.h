#pragma once
#include <vector>

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
    BitMove(int from, int to, int piece, int promotion, bool capture, bool doublepush, bool enpassant, bool castling);

    int get_from() const;
    int get_to() const;

    int get_piece() const;
    int get_promoted() const;

    int get_capture() const;
    int get_doublepush() const;
    int get_enpassant() const;
    int get_castling() const;

    void print();
    std::string get_algebraic_notation();
};

class Board; //FIXME: is there a better way to fix circular header import problems ?
class BitMoveVec : public std::vector<BitMove> {
   public:
    BitMoveVec();
    
    void add_move_if_not_attacked(const BitMove& move, Board & boardObj);
    friend std::ostream& operator<<(std::ostream& os, const BitMoveVec& moves);
};