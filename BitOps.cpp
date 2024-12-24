#include "BitOps.h"

#include <iostream>

#include "Definitions.h"

// OPTI: crucial: inline ? static ?
U64 BitOps::get_bit(const U64& bitboard, int square) {
    return (bitboard >> square) & 1ULL;
}

void BitOps::set_bit(U64& bitboard, int square) {
    bitboard |= (1ULL << square);
}

void BitOps::clear_bit(U64& bitboard, int square) {
    bitboard &= ~(1ULL << square);
}

void BitOps::move_bit(U64& bb, int from, int to) {
    BitOps::clear_bit(bb, from);  // remove piece from old square
    BitOps::set_bit(bb, to);  // add piece to new square
}

int BitOps::rf_to_square(int rank, int file) {
    return 8 * rank + file;
}

void BitOps::square_to_rf(int square, int& rank, int& file) {
    rank = square / 8;
    file = square % 8;
}

// OPTI //  https://www.geeksforgeeks.org/inline-functions-cpp/ reduce call overhead, kinda like macro
int BitOps::count_bits(U64 bb) {
    int nbits = 0;
    while (bb) {
        nbits++;
        // set LSB to 0
        bb &= bb - 1;
    }
    return nbits;
}

// OPTI
int BitOps::get_lsb_index(U64 bb) {
    if (!bb)
        return -1;  // if bitboard is zero, bad
    return BitOps::count_bits((bb & -bb) - 1);
}

void BitOps::display_bits(const U64& bitboard) {
    std::cout << std::endl << "BB repr: " << bitboard << std::endl;
    for (int rank = 0; rank < 8; rank++) {
        std::cout << std::to_string(8 - rank) << "  ";
        for (int file = 0; file < 8; file++) {
            int square = 8 * rank + file;
            std::cout << " " << BitOps::get_bit(bitboard, square);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "   ";
    for (int file = 0; file < 8; file++) {
        std::cout << " " << (char)('a' + file);
    }
    std::cout << std::endl;
};
