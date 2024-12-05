#include "Bishop.h"
#include <cmath>

Bishop::Bishop(int color, Pos pos) {
	this->type = 5;
	this->color = color;
	this->pos = pos;
}

bool Bishop::potential_move(Pos to) {
    for (int i = 1; i < )
	if ( abs(to.x - pos.x) <= 1 && abs(to.y - pos.y) <= 1 )
		return true;
	return false;
}
