#pragma once

#include "Definitions.h"
#include <cstdint>

#define EDGE 0xff818181818181ff
#define CORNER 0x8100000000000081

// Bit operators

constexpr inline Squares lsb(uint64_t x) {
    return static_cast<Squares>(__builtin_ctzll(x));
}

constexpr inline Squares lsb_pop(uint64_t &x) {
    const int t = lsb(x);
    x ^= static_cast<uint64_t>(1) << t;
    return static_cast<Squares>(t);
}

 
constexpr inline Squares msb(uint64_t x) { 
    return static_cast<Squares>(63 ^ __builtin_clzll(x));
}

constexpr inline int popcount(uint64_t x) {
    return __builtin_popcountll(x);
}


// Updated BitBoard implementation, inspired by stockfish

constexpr std::array<Squares, 2> CORNER_A = {a1, a8};
constexpr std::array<Squares, 2> CORNER_H = {h1, h8};

// precomputed things
extern const std::array<std::array<U64, 8>, 64> RAYS;
extern const std::array<std::array<U64, 64>, 64> SQRAYS;
extern const std::array<std::array<U64, 64>, 64> XRAYS;
extern const std::array<std::array<U64, 64>, 6> BABS;
extern const std::array<std::array<U64, 8>, 64> RINGS;
extern const std::array<std::array<U64, 64>, 6> ATTACKS;
extern const std::array<std::array<U64, 64>, 2> PAWN_ATTACKS;
extern const std::array<std::array<U64, 64>, 2> PAWN_PASS;
extern const std::array<U64, 64> PAWN_ISOLATION;

// clang-format off

constexpr inline U64 operator&(Column l, Column r) { return static_cast<U64>(l) & static_cast<U64>(r); }
constexpr inline U64 operator|(Column l, Column r) { return static_cast<U64>(l) | static_cast<U64>(r); }
constexpr inline U64 operator^(Column l, Column r) { return static_cast<U64>(l) ^ static_cast<U64>(r); }
constexpr inline U64 operator&(U64 bb, Column c) { return bb & static_cast<U64>(c); }
constexpr inline U64 operator|(U64 bb, Column c) { return bb | static_cast<U64>(c); }
constexpr inline U64 operator^(U64 bb, Column c) { return bb ^ static_cast<U64>(c); }
constexpr inline U64 operator&=(U64 bb, Column c) { return bb &= static_cast<U64>(c); }
constexpr inline U64 operator|=(U64 bb, Column c) { return bb |= static_cast<U64>(c); }
constexpr inline U64 operator^=(U64 bb, Column c) { return bb ^= static_cast<U64>(c); }
constexpr inline U64 operator&(Column c, Squares sq) { return static_cast<U64>(c) & sq; }
constexpr inline U64 operator|(Column c, Squares sq) { return static_cast<U64>(c) | sq; }
constexpr inline U64 operator^(Column c, Squares sq) { return static_cast<U64>(c) ^ sq; }
constexpr inline U64 operator&(Squares sq, Column c) { return c & sq; }
constexpr inline U64 operator|(Squares sq, Column c) { return c | sq; }
constexpr inline U64 operator^(Squares sq, Column c) { return c ^ sq; }

constexpr inline U64 operator&(Row l, Row r) { return static_cast<U64>(l) & static_cast<U64>(r); }
constexpr inline U64 operator|(Row l, Row r) { return static_cast<U64>(l) | static_cast<U64>(r); }
constexpr inline U64 operator^(Row l, Row r) { return static_cast<U64>(l) ^ static_cast<U64>(r); }
constexpr inline U64 operator&(U64 bb, Row r) { return bb & static_cast<U64>(r); }
constexpr inline U64 operator|(U64 bb, Row r) { return bb | static_cast<U64>(r); }
constexpr inline U64 operator^(U64 bb, Row r) { return bb ^ static_cast<U64>(r); }
constexpr inline U64 operator&=(U64 bb, Row r) { return bb &= static_cast<U64>(r); }
constexpr inline U64 operator|=(U64 bb, Row r) { return bb |= static_cast<U64>(r); }
constexpr inline U64 operator^=(U64 bb, Row r) { return bb ^= static_cast<U64>(r); }
constexpr inline U64 operator&(Row r, Squares sq) { return static_cast<U64>(r) & sq; }
constexpr inline U64 operator|(Row r, Squares sq) { return static_cast<U64>(r) | sq; }
constexpr inline U64 operator^(Row r, Squares sq) { return static_cast<U64>(r) ^ sq; }
constexpr inline U64 operator&(Squares sq, Row r) { return r & sq; }
constexpr inline U64 operator|(Squares sq, Row r) { return r | sq; }
constexpr inline U64 operator^(Squares sq, Row r) { return r ^ sq; }

constexpr inline U64 operator&(Column c, Row r) { return static_cast<U64>(c) & static_cast<U64>(r); }
constexpr inline U64 operator|(Column c, Row r) { return static_cast<U64>(c) | static_cast<U64>(r); }
constexpr inline U64 operator^(Column c, Row r) { return static_cast<U64>(c) ^ static_cast<U64>(r); }
constexpr inline U64 operator&(Row r, Column c) { return c & r; }
constexpr inline U64 operator|(Row r, Column c) { return c | r; }
constexpr inline U64 operator^(Row r, Column c) { return c ^ r; }

// clang-format on

template <Directions D>
constexpr inline U64 Shift(U64 bb) {
    if constexpr (D == N)
        return bb << 8;
    else if constexpr (D == E)
        return bb << 1;
    else if constexpr (D == S)
        return bb >> 8;
    else if constexpr (D == W)
        return bb >> 1;
    else if constexpr (D == NE)
        return bb << 9;
    else if constexpr (D == NW)
        return bb << 7;
    else if constexpr (D == SE)
        return bb >> 7;
    else if constexpr (D == SW)
        return bb >> 9;
    else
        return bb;
}

template <Directions D>
constexpr inline U64 ShiftM(U64 &bb) {
    return bb = Shift<D>(bb);
}

constexpr inline U64 Shift(U64 bb, Directions d) {
    switch (d) {
    case N: return Shift<N>(bb);
    case E: return Shift<E>(bb);
    case S: return Shift<S>(bb);
    case W: return Shift<W>(bb);
    case NE: return Shift<NE>(bb);
    case NW: return Shift<NW>(bb);
    case SE: return Shift<SE>(bb);
    case SW: return Shift<SW>(bb);
    case NODIREC: return Shift<NODIREC>(bb);
    }
    throw std::invalid_argument("Not a valid direction");
}

constexpr inline U64 Shift(U64 bb, Directions d, size_t count) {
    for (size_t i = 0; i < count; i++)
        bb = Shift(bb, d);
    return bb;
}

constexpr inline U64 ShiftM(U64 &bb, Directions d) { return bb = Shift(bb, d); }

constexpr inline U64 ShiftM(U64 &bb, Directions d, size_t count) {
    for (size_t i = 0; i < count; i++)
        bb = Shift(bb, d);
    return bb;
}

constexpr inline bool Multiple(U64 bb) { return bb & (bb - 1); }

constexpr inline U64 Ray(Squares sq, Directions d) {
    return RAYS[sq][static_cast<size_t>(d)];
}

constexpr inline U64 Ray(Squares from, Squares to) {
    return SQRAYS[from][to];
}

constexpr inline U64 XRay(Squares from, Squares to) {
    return XRAYS[from][to];
}

constexpr inline U64 BAndB(Squares sq, Pieces p) {
    return BABS[p][sq];
}

constexpr inline U64 Ring(Squares sq, size_t offset) {
    return RINGS[sq][offset];
}

constexpr inline U64 KnightAttacks(Squares sq) { return ATTACKS[KNIGHT][sq]; }

constexpr inline U64 BishopAttacks(Squares sq) { return ATTACKS[BISHOP][sq]; }

constexpr inline U64 RookAttacks(Squares sq) { return ATTACKS[ROOK][sq]; }

constexpr inline U64 KingAttacks(Squares sq) { return ATTACKS[KING][sq]; }

// All pieces except pawn moves
constexpr inline U64 Attacks(Squares sq, Pieces p) {
    return ATTACKS[p][sq];
}

// Moves for pawn
constexpr inline U64 PawnAttacks(Squares sq, Colors color) {
    return PAWN_ATTACKS[color][sq];
}

constexpr inline U64 PawnPassMask(Squares sq, Colors color) {
    return PAWN_PASS[color][sq];
}
constexpr inline U64 PawnIsolationMask(Squares sq) {
    return PAWN_ISOLATION[sq];
}
