#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <array>
#include <cstring>

#include "Definitions.h"

// OPTI: (would uint32_t be better ? speed ?)
/*  move template (ULL is 64 bits)
    this encodes relevant move information in a single 64 bit integer
    0000 0000 0000 0000 0000 0000 0011 1111   from square
    0000 0000 0000 0000 0000 1111 1100 0000   to square
    0000 0000 0000 0000 1111 0000 0000 0000   piece code
    0000 0000 0000 1111 0000 0000 0000 0000   promoted piece code
    0000 0000 0001 0000 0000 0000 0000 0000   is capture ?
    0000 0000 0010 0000 0000 0000 0000 0000   is double pawn push ?
    0000 0000 0100 0000 0000 0000 0000 0000   is enpassant ?
    0000 0000 1000 0000 0000 0000 0000 0000   is castling ?
    N.B: still have 40 free bits to use
*/
class BitMove {

   public:
    U64 m_bit = 0ULL;
    BitMove() noexcept = default;
    BitMove(int bitMove);
    BitMove(int from, int to, int piece, int promotion, bool capture, bool doublepush, bool enpassant, bool castling);

    int get_from() const;
    int get_to() const;

    int get_piece() const;
    int get_promotion_piece() const;

    int get_capture() const;
    int get_doublepush() const;
    int get_enpassant() const;
    int get_castling() const;

    void print();
    std::string get_algebraic_notation() const;

    bool operator==(const BitMove &move) const noexcept {return this->m_bit == move.m_bit;}
    bool is_defined() const noexcept {return m_bit != 0;}

    inline U64 get_bit_repr() const {
        return m_bit;
    }
};

class Board;  // FIXME: is there a better way to fix circular header import problems ?

class BitMoveVec : public std::vector<BitMove> {
   public:
    BitMoveVec();

    friend std::ostream& operator<<(std::ostream& os, const BitMoveVec& moves);
};

struct MoveList { // new move list
    enum Type { Quiet, Attack };
    size_t size() const { return attack_count + quiet_count; }
    size_t attacks() const { return attack_count; }
    bool empty() const { return size() == 0; }
    BitMove &operator[](size_t i) { return moves[i]; }
    const BitMove &operator[](size_t i) const { return moves[i]; }
    template <Type t>
    void push(BitMove move) {
        if constexpr (t == Attack) {
            moves[attack_count++] = move;
        } else {
            moves[256 - (++quiet_count)] = move;
        }
    }
    void finish() {
        std::memmove(
            &moves[attack_count], &moves[256 - quiet_count], quiet_count * sizeof(BitMove)
        );
    }
    std::array<BitMove, 256>::iterator begin() { return &moves[0]; }
    std::array<BitMove, 256>::const_iterator begin() const { return &moves[0]; }
    std::array<BitMove, 256>::iterator end() { return &moves[size()]; }
    std::array<BitMove, 256>::const_iterator end() const { return &moves[size()]; }

private:
    std::array<BitMove, 256> moves;
    size_t attack_count = 0;
    size_t quiet_count  = 0;
};
