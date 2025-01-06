#include "Board.h"
#define MAX_ALPHA_BETA_DEPTH 4

struct Game {
	OldBoard my_board;
	int turn;
	std::string my_move;
	Game();

    double search_best(int depth, int t, double prev_eval);
	double evaluate();
	double evaluate_piece(Piece* piece); 
	int search(int depth, int t);
	int search_best_alpha_beta(int depth, int t, int prev_eval, int alpha, int beta);
};
