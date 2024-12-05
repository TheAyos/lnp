#include "Piece.h"

struct King : Piece {
	bool is_castled;

	King(int color, Pos pos);

	bool potential_move(Pos to);
};
