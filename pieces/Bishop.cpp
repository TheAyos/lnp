#include "Bishop.h"
#include <cmath>

Bishop::Bishop(int color, Pos pos) {
	this->type = 2;
	this->color = color;
	this->pos = pos;
}

bool Bishop::potential_move(Pos to) {
	if ( abs(to.x - pos.x) == abs(to.y - pos.y) )
		return true;
	return false;
}
