#pragma once
#include "Piece.h"

struct Pawn : Piece {
    Pos original_pos;
    

	Pawn(int color, Pos pos);

    bool has_moved();
    //bool en_passant();
    bool check_promotion();
    void promotion(Piece* board[8][8]);
	std::vector<std::string> legal_moves(Piece* board[8][8]);	
};
