#include "Board.h"

struct Game {
	Board my_board;
	int turn;
	std::string my_move;
	Game();

	double search_best(int depth, int t, double prev_eval);
	double evaluate();
	double evaluate_piece(Piece* piece); 
	int search(int depth, int t);
};
