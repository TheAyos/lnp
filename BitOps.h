#pragma once
#include <iostream>

#include "BitIterator.h"
#include "Definitions.h"

#ifdef _MSC_VER
    #include <immintrin.h>
    #include <intrin.h>
#endif

namespace BitOps {
    void display_bits(const U64& bitboard);

    // resources:
    // https://www.geeksforgeeks.org/inline-functions-cpp/ reduce call overhead, kinda like macro
    // https://isocpp.org/wiki/faq/inline-functions#inline-member-fns
    // https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html CPUs have fast instructions for bit operations

    static inline U64 get_bit(const U64& bitboard, int square) {
        return (bitboard >> square) & 1ULL;
    }

    static inline void set_bit(U64& bitboard, int square) {
        bitboard |= (1ULL << square);
    }

    static inline void clear_bit(U64& bitboard, int square) {
        bitboard &= ~(1ULL << square);
    }

    static inline void move_bit(U64& bb, int from, int to) {
        // it's just clear_bit and set_bit merged
        bb = (bb & ~(1ULL << from)) | (1ULL << to);
    }

    static inline int rf_to_square(int rank, int file) {
        return 8 * rank + file;
    }

    static inline void square_to_rf(int square, int& rank, int& file) {
        rank = square / 8;
        file = square % 8;
    }

    static inline int pop_count_bits(U64 bb) {
        // to be able to compile and use Visual Studio debug tools
        #ifdef _MSC_VER
            return _mm_popcnt_u64(bb);
        #else
            return __builtin_popcountll(bb);
        #endif
    }

    static inline int get_lsb_index(U64 bb) {
        if (!bb) return -1;  // if bitboard is zero, bad
        // to be able to compile and use Visual Studio debug tools
        #ifdef _MSC_VER
            return _tzcnt_u64(bb);
        #else
            return __builtin_ctzll(bb);
        #endif
    }

    inline U64 get_sliding_blockers(int index, int n_mask_bits, U64 attack_mask) {
        U64 blockers = 0ULL;
        for (int i = 0; i < n_mask_bits; i++) {
            int sq = get_lsb_index(attack_mask);
            clear_bit(attack_mask, sq);
            if (index & (1 << i)) blockers |= (1ULL << sq);
        }
        return blockers;
    }

}  // namespace BitOps