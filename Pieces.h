#include <iostream>
#include "Square.h"
#include <vector>

#ifndef PIECES_H
#define PIECES_H

struct Pieces {
	virtual std::vector<Square> potential_moves(Square Board[8][8]) = 0;
	int my_row, my_col;
	int my_color, my_type;
	bool is_Alive;
};

#endif
