#include "Square.h"
#include "Pieces.h"

struct King : Pieces {
	King(int color);
	// bool is_Check();
	std::vector<Square> potential_moves(Square Board[8][8]);
};
