#ifndef SQUARE_H
#define SQUARE_H

#include "Piece.h"

struct Square {
	Piece* piece;

	Square() : piece { nullptr } {}
	
	Piece* get_piece();
	void set_piece(Piece* piece);
	void clean();
};	

#endif
