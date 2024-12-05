#include "Piece.h"

struct Bishop : Piece {
	King(int color, Pos pos);

	bool potential_move(Pos to);
};
