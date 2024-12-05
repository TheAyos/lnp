#include "Pawn.h"

Pawn::Pawn(int color, Pos pos) {
	this->type = 0;
	this->color = color;
	this->pos = pos;
	this->has_moved = false;
}

bool Pawn::potential_move(Pos to) {
	return false;
}
