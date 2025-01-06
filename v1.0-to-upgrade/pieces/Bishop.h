#ifndef BISHOP_H
#define BISHOP_H

#include "bitpieces/Piece.h"

struct Bishop : Piece {
    Bishop(int color, Pos pos);

    std::vector<std::string> legal_moves(Piece* board[8][8]);
};

#endif
