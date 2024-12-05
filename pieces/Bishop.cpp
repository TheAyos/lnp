#include "Bishop.h"
#include <cmath>

Bishop::Bishop(int color, Pos pos) {
	this->type = 2;
	this->color = color;
	this->pos = pos;
}

std::vector<std::string> Bishop::legal_moves(Piece* board[8][8]) {
	exit(0);
}
