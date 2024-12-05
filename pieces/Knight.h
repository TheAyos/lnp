#include "Piece.h"

struct Knight : Piece {
	Knight(int color, Pos pos);

	bool potential_move(Pos to);
};
