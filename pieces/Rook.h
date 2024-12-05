#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

struct Rook : Piece {
    Rook(int color, Pos pos);
    bool has_moved;

    bool potential_move(Pos to);
};

#endif