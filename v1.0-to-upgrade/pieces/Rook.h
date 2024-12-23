#pragma once

#include "bitpieces/Piece.h"

struct Rook : Piece {
    Rook(int color, Pos pos);
    // bool has_moved;

    std::vector<std::string> legal_moves(Piece* board[8][8]); 
};