#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

struct Queen : Piece {
    Queen(int color, Pos pos);

    bool potential_move(Pos to);
};

#endif