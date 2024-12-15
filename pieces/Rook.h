#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

struct Rook : Piece {
    Rook(int color, Pos pos);
    // bool has_moved;

    std::vector<std::string> legal_moves(Piece* board[8][8]); 
};

#endif
