#include "Piece.h"

struct Pawn : Piece {
    Pos original_pos;
    

	Pawn(int color, Pos pos);

    bool has_moved;
    //bool en_passant();
    //bool promotion();
	bool potential_move(Pos to);
};
