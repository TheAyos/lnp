#include "BitOps.h"

#include <iostream>

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
