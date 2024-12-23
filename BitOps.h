#pragma once
#include <iostream>

#include "Definitions.h"
#include "BitIterator.h"


namespace BitOps {
    U64 get_bit(const U64& bitboard, int square);

    void set_bit(U64& bitboard, int square);

    void clear_bit(U64& bitboard, int square);

    void move_bit(U64& bb, int from, int to);

    int rf_to_square(int rank, int file);

    void square_to_rf(int square, int& rank, int& file);

    int count_bits(U64 bb);

    int get_lsb_index(U64 bb);

    void display_bits(const U64& bitboard);
}  // namespace BitOps