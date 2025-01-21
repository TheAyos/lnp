#include "Bitboard.h"
#include "Definitions.h"

constexpr std::array<std::array<U64, 8>, 64> RAYS = [] {
    auto rays = decltype(RAYS){};
    for (const auto sq : SQUARES)
        for (const auto dir : DIRECTIONS) {
            U64 ray = 0;

            U64 dot = ToBB(sq);
            while (dot & (~EDGES[static_cast<size_t>(dir)]))
                ray |= ShiftM(dot, dir);

            rays[static_cast<size_t>(sq)][static_cast<size_t>(dir)] = ray;
        }
    return rays;
}();

constexpr std::array<std::array<U64, 64>, 64> SQRAYS = [] {
    auto rays = decltype(SQRAYS){};
    for (const auto from : SQUARES)
        for (const auto to : SQUARES) {
            if (from == to) continue;
            const Directions dir = Util::GetDirection(from, to);
            rays[static_cast<size_t>(from)][static_cast<size_t>(to)] = Ray(from, dir);
        }
    return rays;
}();

constexpr std::array<std::array<U64, 64>, 64> XRAYS = [] {
    auto rays = decltype(XRAYS){};
    for (const auto from : SQUARES)
        for (const auto to : SQUARES) {
            if (from == to) continue;
            const Directions dir = Util::GetDirection(from, to);
            rays[static_cast<size_t>(from)][static_cast<size_t>(to)] = Ray(to, dir);
        }
    return rays;
}();

namespace {
constexpr U64 GenerateRing(Squares sq, size_t offset) {
    U64 ring = 0;

    const std::array<Directions, 4> directions{N, E, S, W};
    const std::array<std::array<Directions, 2>, 4> probes{
        std::array{W, E}, std::array{N, S}, std::array{W, E},
        std::array{N, S}
    };

    for (size_t i = 0; i < directions.size(); ++i) {
        U64 dot = ToBB(sq);
        for (size_t o = 0; o < offset; ++o) {
            if (dot & (EDGES[static_cast<size_t>(directions[i])])) {
                dot = 0;
                break;
            }
            dot = Shift(dot, directions.at(i));
            for (const auto dir : probes.at(i))
                if (!(dot & (EDGES[static_cast<size_t>(dir)]))) dot |= Shift(dot, dir);
        }
        ring |= dot;
    }

    return ring;
}
} // namespace
constexpr std::array<std::array<U64, 8>, 64> RINGS = [] {
    auto rings = decltype(RINGS){};

    for (const auto sq : SQUARES)
        for (size_t i = 1; i < 8; i++)
            rings[static_cast<size_t>(sq)][i] = GenerateRing(sq, i);

    return rings;
}();

namespace {
constexpr U64 GenerateAttacks(Pieces piece, Squares sq) {
    switch (piece) {
    case KNIGHT:
        return Ring(sq, 2) & ~(Ray(sq, N) | Ray(sq, E) | Ray(sq, S) | Ray(sq, W) |
                               Ray(sq, NW) | Ray(sq, NE) | Ray(sq, SW) |
                               Ray(sq, SE));
    case KING: return Ring(sq, 1);
    case ROOK: return Ray(sq, N) | Ray(sq, E) | Ray(sq, S) | Ray(sq, W);
    case BISHOP:
        return Ray(sq, NW) | Ray(sq, NE) | Ray(sq, SW) |
               Ray(sq, SE);
    case QUEEN:
        return Ray(sq, N) | Ray(sq, E) | Ray(sq, S) | Ray(sq, W) |
               Ray(sq, NW) | Ray(sq, NE) | Ray(sq, SW) |
               Ray(sq, SE);
    case PAWN:
    case NOPIECE:
        throw std::invalid_argument("Cannot generate");
    }
}
} // namespace

constexpr std::array<std::array<U64, 64>, 6> ATTACKS = [] {
    auto attacks = decltype(ATTACKS){};

    for (const auto p : PIECES)
        for (const auto sq : SQUARES)
            if (p != PAWN) attacks[p][sq] = GenerateAttacks(p, sq);

    return attacks;
}();

constexpr std::array<std::array<U64, 64>, 6> BABS = [] {
    auto babs = decltype(BABS){};

    for (const auto pType : PIECES) {
        if (pType == PAWN || pType == KNIGHT || pType == KING) continue;
        for (const auto sq : SQUARES) {
            U64 nonEdge;
            if ((sq & EDGE) == 0)
                nonEdge = ~EDGE;
            else if (sq & CORNER)
                nonEdge = ~CORNER;
            else if (sq & Row::R1)
                nonEdge = ~(Row::R8 | Column::a | Column::h);
            else if (sq & Row::R8)
                nonEdge = ~(Row::R1 | Column::a | Column::h);
            else if (sq & Column::a)
                nonEdge = ~(Row::R1 | Row::R8 | Column::h);
            else if (sq & Column::h)
                nonEdge = ~(Row::R1 | Row::R8 | Column::a);
            else
                throw std::logic_error("Should not happen");
            babs[static_cast<size_t>(pType)][static_cast<size_t>(sq)] =
                Attacks(sq, pType) & nonEdge;
        }
    }
    return babs;
}();

constexpr std::array<std::array<U64, 64>, 2> PAWN_ATTACKS = [] {
    auto attacks = decltype(PAWN_ATTACKS){};

    for (const auto color : {WHITE, BLACK})
        for (const auto sq : SQUARES) {
            const U64 rays = (color == WHITE) ? (Ray(sq, NE) | Ray(sq, NW))
                                             : (Ray(sq, SE) | Ray(sq, SW));

            attacks[static_cast<size_t>(color)][static_cast<size_t>(sq)] = Ring(sq, 1) & rays;
        }

    return attacks;
}();

constexpr std::array<std::array<U64, 64>, 2> PAWN_PASS = [] {
    auto passes = decltype(PAWN_PASS){};

    for (const auto color : {WHITE, BLACK})
        for (const auto square : SQUARES) {
            const Directions dir[2] = {N, S};
            U64 bb                  = 0;

            bb |= Ray(square, dir[static_cast<size_t>(color)]);
            if ((Column::a & square) == 0) {
                U64 b     = Shift<W>(ToBB(square));
                Squares n = lsb(b);
                bb |= Ray(n, dir[static_cast<size_t>(color)]);
            }
            if ((Column::h & square) == 0) {
                U64 b     = Shift<E>(ToBB(square));
                Squares n = lsb(b);
                bb |= Ray(n, dir[static_cast<size_t>(color)]);
            }

            passes[static_cast<size_t>(color)][static_cast<size_t>(square)] = bb;
        }

    return passes;
}();

constexpr std::array<U64, 64> PAWN_ISOLATION = [] {
    auto values = decltype(PAWN_ISOLATION){};

    for (const auto square : SQUARES) {
        U64 bb = 0;

        const size_t index = Util::GetColumnIndex(square);
        if (index > 0) bb |= COLUMNS[index - 1];
        if (index < 7) bb |= COLUMNS[index + 1];

        values[static_cast<size_t>(square)] = bb;
    }
    return values;
}();
