#pragma once
#include "Piece.h"

struct Pawn : Piece {
    Pos original_pos;
    
	bool has_moved1();
	Pawn(int color, Pos pos);

	std::vector<std::string> legal_moves(Piece* board[8][8]);	
};
