#pragma once
#include "Piece.h"

struct Pawn : Piece {
    Pos original_pos;
    

	Pawn(int color, Pos pos);

    bool has_moved();
    //bool en_passant();
    //bool promotion();
	std::vector<std::string> legal_moves(Piece* board[8][8]);	
};
