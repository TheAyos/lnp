#ifndef SQUARE_H
#define SQUARE_H

struct Square {
	int row, col;
	int piece_id; // piece identifier
	/* -1: empty
	 * 1: pawn
	 * 2: knight
	 * 3: bishop
	 * 4: rook
	 * 5: queen
	 * 6: king
	 */
	int color_id; // color identifier
	/*
	 * -1: empty
	 * 0: black
	 * 1: white
	 */
	Square();
};	

#endif
