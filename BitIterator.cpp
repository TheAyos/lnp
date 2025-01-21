#include "BitIterator.h"

#include "BitOps.h"

using namespace BitOps;
void BitIterator::next() {
    if (bb)
        current = pop_count_bits((bb & -bb) - 1);
}

BitIterator::BitIterator(U64 bitboard) : bb(bitboard) {
    next();
}

BitIterator::BitIterator() : bb(0) {
}

int BitIterator::operator*() const {
    return current;
}

BitIterator& BitIterator::operator++() {
    if (bb) {
        bb &= bb - 1;  // clear LSB
        next();
    }
    return *this;
}

bool BitIterator::operator!=(const BitIterator& other) const {
    return bb != other.bb;
}

inline BitIterator BitIterator::begin(U64 bitboard) {
    return BitIterator(bitboard);
}

inline BitIterator BitIterator::end() {
    return BitIterator(0);
}