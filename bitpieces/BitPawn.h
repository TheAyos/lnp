#pragma once
#include <iostream>
#include "../BitBoard.h"

class BitPawn {
    public:
        int color;
        BitPawn();
        BitPawn(int color);
        static U64 get_attack_masks(int colo, int square);
};

BitPawn::BitPawn() : color(W) {};
BitPawn::BitPawn(int color) : color(color) {};


U64 BitPawn::get_attack_masks(int color, int square) {
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