#pragma once
#include "bitpieces/Piece.h"

struct Knight : Piece {
	Knight(int color, Pos pos);

	std::vector<std::string> legal_moves(Piece* board[8][8]);
};
