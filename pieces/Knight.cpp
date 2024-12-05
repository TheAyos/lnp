#include "Knight.h"
#include <cmath>

Knight::Knight(int color, Pos pos) {
	this->type = 1;
	this->color = color;
	this->pos = pos;
}

bool Knight::potential_move(Pos to) {
	if ( abs(to.x - pos.x) == 1 && abs(to.y - pos.y) == 2) || ( abs(to.x - pos.x) == 2 && abs(to.y - pos.y) == 1)
		return true;
	return false;
}
