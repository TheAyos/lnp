#include "King.h"
#include <cmath>

King::King(int color, Pos pos) {
  this->type = 5;
  this->color = color;
  this->pos = pos;
  is_castled = false;
}

std::vector<std::string> King::legal_moves(Piece* board[8][8]) {
	exit(0);
}
