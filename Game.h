#include "King.h"

struct Game {
	Square Board[8][8];
	std::vector<Pieces*> w_Pieces;
	std::vector<Pieces*> b_Pieces;
	bool game_over;
	int turn; // 1 for white, 0 for black
	Game();
	std::vector<Square> get_moves(Pieces* p);
	void move(Pieces* p, int row, int col);
	void display();
};
