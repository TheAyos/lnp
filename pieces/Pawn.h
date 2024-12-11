#pragma once
#include "Piece.h"
#include "Queen.h"

struct Pawn : Piece {
    Pos original_pos;
    

	Pawn(int color, Pos pos);

    bool has_moved();
    //bool en_passant();
    bool check_promotion(Pos pos);
	std::vector<std::string> legal_moves(Piece* board[8][8]);	
};
