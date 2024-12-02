#include "King.h"
#include "Square.h"
#include "Pieces.h"

King::King(int color) {
	my_color = color;
	is_Alive = true;
	my_col = 4;
	my_row = my_color ? 0 : 7;
	my_type = 6;
}

std::vector<Square> King::potential_moves(Square Board[8][8]) {
	std::vector<Square> moves;
	int drow[] = {1,1,1,0,0,-1,-1,-1};
	int dcol[] = {1,0,-1,1,-1,1,0,-1};
	for (int i = 0; i < 8; i++) {
		int nrow = my_row+drow[i];
		int ncol = my_col+dcol[i];
		// std::cout << nrow << " " << ncol << std::endl;
		if (nrow > 7 || nrow < 0 || ncol > 7 || nrow < 0)
			continue;
		if (Board[nrow][ncol].color_id == my_color)
			continue;
		
		moves.push_back(Board[nrow][ncol]);
	}
	return moves;
}
