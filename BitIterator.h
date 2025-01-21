#pragma once
#include "Board.h"

// to be able to iterate over the bits of a bitboard
// e.g. for (auto it = BitOps::begin(bb); it != BitOps::end(); ++it) {
//          int square = *it;
//          ...
//      }
class BitIterator {
   private:
    U64 bb;
    int current;

    void next();

   public:
    BitIterator(U64 bitboard);

    BitIterator();

    int operator*() const;

    BitIterator& operator++();

    bool operator!=(const BitIterator& other) const;
    inline BitIterator begin(U64 bitboard);

    inline BitIterator end();
};