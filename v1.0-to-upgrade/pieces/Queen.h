#ifndef QUEEN_H
#define QUEEN_H

#include "bitpieces/Piece.h"

struct Queen : Piece {
    Queen(int color, Pos pos);
    bool was_pawn; //for promotion
    
    std::vector<std::string> legal_moves(Piece* board[8][8]); 
};

#endif
