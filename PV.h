#pragma once

#include "BitMove.h"
#include <vector>

struct PV {
public:
    PV() : _ply(0) {}
    PV(size_t ply) : _ply(ply) {}
    PV(size_t ply, std::vector<BitMove> moves) : _moves(moves), _ply(ply) {}

    inline bool empty() const { return _moves.empty(); }
    inline size_t size() const { return _moves.size(); }
    inline size_t ply() const { return _ply; }

    inline BitMove &operator[](size_t i) { return _moves[i]; }
    inline const BitMove &operator[](size_t i) const { return _moves[i]; }

private:
    std::vector<BitMove> _moves;
    size_t _ply;
};
