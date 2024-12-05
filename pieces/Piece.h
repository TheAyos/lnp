#include <iostream>

#ifndef PIECE_H
#define PIECE_H

struct Pos {
	int x;
	int y;
};

struct Piece {
	Pos pos;
	int color, type;
	/*
	 * color: 0 black, 1 white
	 * type: 0 pawn, 1 knight, 2 bishop, 3 rook, 4 queen, 5 king
	 * pos refers to position
	 */

	Piece() : pos {0,0}, color{0}, type{0} {}

	void set_pos(Pos pos);
	Pos get_pos();
	int get_color();
	int get_type();
	virtual bool potential_move(Pos to) = 0;
};

#endif
