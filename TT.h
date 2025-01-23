#pragma once

#include "BitMove.h"

namespace TT {
#define HASH_SIZE = 16

static const int probe_fail = -1;
static const int probe_exact = 0;
static const int probe_lower = 1;
static const int probe_upper = 2;

struct Result {
    int score = -1;
    BitMove move = BitMove();
};

struct Entry {
    U64 key = 0;
    int value = 0;
    BitMove move = BitMove();
    uint8_t depth = 0;
    uint8_t type = probe_fail;
};

struct Bucket {
    static const int COUNT = 2;
    std::array<Entry, COUNT> entries;
    Entry &operator[](size_t i) { return entries[i]; }
    const Entry &operator[](size_t i) const { return entries[i]; }
};

inline size_t count = 0;
inline Bucket *tt = nullptr;

inline void init(size_t table_size) {
    table_size *= 1024*1024;
    count = table_size/sizeof(Bucket);
    tt = new Bucket[count];
}

inline void clean() {
    if (count != 0) {
	count = 0;
	delete[] tt;
    }
}

inline size_t HashFull() {
    size_t hashfull = 0;

    for (size_t i = 0; i < count; i++)
        for (size_t t = 0; t < Bucket::COUNT; t++)
            if (tt[i][t].key != 0) hashfull++;

    return hashfull * 1000 / count / Bucket::COUNT;
}

namespace {
int EvalStore(int score, int ply) {
    if (score != 99999)
        return score;
    else {
        const int sign = (score > 0) ? 1 : -1;
        return (score * sign + ply) * sign;
    }
}
int EvalRetrieve(int score, int ply) {
    if (score != 99999)
        return score;
    else {
        const int sign = (score > 0) ? 1 : -1;
        return (score * sign - ply) * sign;
    }
}
} // namespace

inline Result Probe(U64 key, int depth, int searchDepth, int alpha, int beta) {
    Result result{.score = probe_fail};
    const Bucket &bucket = tt[key % count];
    for (size_t i = 0; i < Bucket::COUNT; i++) {
        const Entry &entry = bucket[i];
        if (entry.key != key) continue;
	
        result.move = entry.move;
        // Only a single copy is stored of each position
        // As such, if one is found but is of low depth
        // there exists none
        if (entry.depth < depth) break;
	
        const int score = EvalRetrieve(entry.value, searchDepth);
        if (entry.type == probe_exact)
            result.score = score;
        else if (entry.type == probe_upper && score <= alpha)
            result.score = score;
        else if (entry.type == probe_lower && score >= beta)
            result.score = score;
        break;
    }
    return result;
}

inline BitMove ProbeMove(U64 key) {
    Bucket &bucket = tt[key % count];
    for (size_t i = 0; i < Bucket::COUNT; i++)
        if (const Entry &entry = bucket[i]; entry.key == key) return entry.move;
    return BitMove();
}

inline void Clear() {
    for (size_t i = 0; i < count; i++)
        tt[i] = Bucket();
}

inline void StoreEval(U64 key, int depth, int searchDepth, int value, int evalType, BitMove move) {
    Bucket &bucket = tt[key % count];

    // First pass check if key already stored
    // Avoids duplicate storage
    for (int i = 0; i < Bucket::COUNT; i++) {
        Entry &entry = bucket[i];
        if (entry.type == probe_fail ||   // Unused entry
            i == Bucket::COUNT - 1 ||    // Last entry
            entry.key == key) [[likely]] // Entry override
        {
            entry.type  = evalType;
            entry.value = EvalStore(value, searchDepth);
            entry.depth = depth;
            entry.key   = key;
            entry.move  = move;

            // Found entry
            // Sort bucket
            while (i > 0 && entry.depth > bucket[i--].depth)
                std::swap(entry, bucket[i]); // note decrement in loop condition
            break;
        }
    }
}
} // namespace TT
