#include "Board.h"

struct Game {
	Board my_board;
	int turn;
	Game();
	int search(int depth, int t);
};
