#include "Game.h"

Game::Game(){	 
	turn = 1;
}

int Game::search(int depth, int t) {
	std::vector<std::string> cache = my_board.all_legal_moves(t);
	int out = 0;

	if (depth == 1) return cache.size();	
	
	for (auto & str : cache) {
		Piece* c = my_board.move(my_board.str_from(str), my_board.str_to(str)); 
		out += search(depth-1, 1-t);
		my_board.undo_move(my_board.str_from(str), my_board.str_to(str), c);
	}
	return out;
}
