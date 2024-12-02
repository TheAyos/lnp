#include "Square.h"
#include "Pieces.h"

struct Pawn : Pieces {
	Pawn(int color);
	//have_moved()
	std::vector<Square> potential_moves(Square Board[8][8]);
};
