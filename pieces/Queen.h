#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

struct Queen : Piece {
    Queen(int color, Pos pos);

   std::vector<std::string> legal_moves(Piece* board[8][8]); 
};

#endif
