#include "Rook.h"

Rook::Rook(int color, Pos pos) {
    this->type = 3;
    this->color = color;
    this->pos = pos;
    this->has_moved = false;
}

std::vector<std::string> Rook::legal_moves(Piece* board[8][8]) {
	exit(0);
}
