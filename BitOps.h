#pragma once
#include <iostream>

#include "Definitions.h"

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

void square_to_rf(int square, int& rank, int& file) {
    rank = square / 8;
    file = square % 8;
}

// OPTI //  https://www.geeksforgeeks.org/inline-functions-cpp/ reduce call overhead, kinda like macro
int count_bits(U64 bb) {
    int nbits = 0;
    while(bb) {
        nbits++;
        // set LSB to 0
        bb &= bb - 1;
    }
    return nbits;
}

// OPTI
int index_ls1b(U64 bb) {
    if(!bb)
        return -1;  // if bitboard is zero, bad
    return count_bits((bb & -bb) - 1);
}

void display_bits(const U64& bitboard) {
    std::cout << std::endl << "BB repr: " << bitboard << std::endl;
    for(int rank = 0; rank < 8; rank++) {
        std::cout << std::to_string(8 - rank) << "  ";
        for(int file = 0; file < 8; file++) {
            int square = 8 * rank + file;
            std::cout << " " << get_bit(bitboard, square);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "   ";
    for(int file = 0; file < 8; file++) {
        std::cout << " " << (char)('a' + file);
    }
    std::cout << std::endl;
};