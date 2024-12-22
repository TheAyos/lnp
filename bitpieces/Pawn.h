#pragma once
#include "BitBoard.h"
#include "Piece.h"

class Pawn : Piece {
   public:
    static U64 get_attack_mask(int color, int square);    
};

U64 Pawn::get_attack_mask(int color, int square) {
    U64 attacks = 0ULL; // attacks bitmask
    U64 bb = 0ULL; // bitboard
    set_bit(bb, square); // bit position

    if (color == W) {
        if ((bb >> 7) & not_file_masks[a]) attacks |= (bb >> 7); // capture to the right front & make sure not to wrap around edge of board
        if ((bb >> 9) & not_file_masks[h]) attacks |= (bb >> 9); // capture to the left front & make sure not to wrap around edge of board
    } else {
        if ((bb << 7) & not_file_masks[h]) attacks |= (bb << 7); // ...
        if ((bb << 9) & not_file_masks[a]) attacks |= (bb << 9);
    }

    return attacks;
};