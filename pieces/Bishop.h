#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

struct Bishop : Piece {
    Bishop(int color, Pos pos);

    bool potential_move(Pos to);
};

#endif