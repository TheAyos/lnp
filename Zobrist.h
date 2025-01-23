#pragma once

#include "Definitions.h"
#include <array>
#include <cstdint>
#include <iostream>

constexpr int HASHCOUNT = 2*6*64;

constexpr std::array<U64, HASHCOUNT> generateHashes() {
    // auto table = decltype(HASHES){};
    std::array<U64, HASHCOUNT> table{};

    U64 lfsr = 0x181818ffff181818;
    U64 bit = 0;

    for (int i = 0; i < HASHCOUNT; i++) {
	bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
	lfsr = (lfsr >> 1) | (bit << 63);
	table[i] = lfsr;
    }

    return table;
}

constexpr std::array<U64, HASHCOUNT> HASHES = generateHashes();

namespace Zobrist {

inline void flip_sq(U64 &hash, int square, int type, int color) {
    hash ^= HASHES[(64*6)*color + 64*type + square];
    // std::cout << square << "sq " << type << "type " << color << std::endl;
}

inline void flip_castle(U64 &hash, int color, int side) {
    hash ^= HASHES[HASHCOUNT + 2*color + side];
}

inline void flip_ep(U64 &hash, int square) {
    hash ^= HASHES[HASHCOUNT + 4 + square];
}

inline void flip_color(U64 &hash) {
    hash ^= 0xaa55aa55aa55aa55;
} 
} // namespace Zobrist
