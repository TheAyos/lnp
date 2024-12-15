#include "Game.h"

Game::Game(){	 
	turn = 1;
}

double Game::search_best(int depth, int t, double prev_eval) {
	// if (depth == 3) my_move.clear();

	double value;
	if (t == 1) value = -9999;
	else value = 9999;
	double best_value = 0;

	std::vector<std::string> cache = my_board.all_legal_moves(t);
	
	for (auto & str : cache) {
		double cur_eval = 0;
		
		int board_status[10];
		my_board.move(str,board_status); // make the move
		cur_eval = evaluate();

		if ((t==0 && cur_eval <= prev_eval) || (t==1 && cur_eval >= prev_eval)) {
			if (depth != 1) {
				best_value = search_best(depth-1, 1-t, cur_eval);
				if ((t==0 && best_value < value) || (t==1 && best_value > value)) {
					value = best_value;
					if (depth == 3) my_move = str;
				}

			}
			else if ((t==0 && cur_eval < value) || (t==1 && cur_eval > value)) {
				value = cur_eval;
				if (depth == 3) my_move = str;
			}

		}
		// if (my_move.length() > 0)
			// std::cout << my_move << std::endl;
		my_board.undo_move(str,board_status); //undo move
	}

	if (depth == 3) {
		return 0;
	}
	else return value;
}

double Game::evaluate(){
	double evaluation = 0;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (my_board.board[i][j]!=nullptr) evaluation += evaluate_piece(my_board.board[i][j]);
	
	if (my_board.in_check(1)) {
		evaluation -= 30;
		std::vector<std::string> moves = my_board.all_legal_moves(1);
		if (moves.size() == 0) return -9999;
	}
	else if (my_board.in_check(0)) {
		evaluation += 30;
		std::vector<std::string> moves = my_board.all_legal_moves(0);
		if (moves.size() == 0) return 9999;
	}
	return evaluation;
}

double Game::evaluate_piece(Piece* piece) {
	double p_evaluation = 0;
	int type = piece->type;
	if (type == 0) p_evaluation += 10;
	if (type == 1 || type == 2) p_evaluation += 30;
	if (type == 3) p_evaluation += 50;
	if (type == 4) p_evaluation += 90;
	if (type == 5) p_evaluation += 900;
	return (piece->color)?p_evaluation: 0-p_evaluation;
}

// perft searcher
int Game::search(int depth, int t) {
	std::vector<std::string> cache = my_board.all_legal_moves(t);
	int out = 0;

	if (depth == 1) {
		// for (auto & str : cache) std::cout << str << std::endl;
		return cache.size();
	}

	for (auto & str : cache) {
		int board_status[10];	
		my_board.move(str, board_status);	
		out += search(depth-1, 1-t);
	        my_board.undo_move(str, board_status);	
	}
	return out;
}
