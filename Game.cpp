#include "Game.h"

Game::Game(){	 
	turn = 1;
}

double Game::search_best(int depth, int t, double prev_eval) {
	// Total nodes searched: 433 at depth 3
	// Total nodes searched: 7946 at depth 4
	// if (depth == 3) my_move.clear();
	static long nodes_searched = 0;
	if (depth == MAX_ALPHA_BETA_DEPTH) {
		nodes_searched = 0; // reset at start of new search
	}
	nodes_searched++;

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

	if (depth == MAX_ALPHA_BETA_DEPTH) {
		std::cout << "Total nodes searched: " << nodes_searched << std::endl;
		return 0;
	}
	else return value;
}

int Game::search_best_alpha_beta(int depth, int t, int prev_eval, int alpha, int beta) {
	// Total nodes searched: 433 at depth 3
	// Total nodes searched: 7946 at depth 4
   if (depth == 0) return evaluate();
   
   static long nodes_searched = 0;
   if (depth == MAX_ALPHA_BETA_DEPTH) {
      nodes_searched = 0; // reset at start of new search
   }
   nodes_searched++;

   int bestValue = t ? -999999 : 999999;
   int score = 0;
   
   std::vector<std::string> moves = my_board.all_legal_moves(t);
   for (const auto& move : moves) {
      int board_status[10];
      my_board.move(move, board_status);
      
      score = search_best_alpha_beta(depth-1, 1-t, prev_eval, alpha, beta);
      
      if (t) { // maximizing player
         if (score > bestValue) {
            bestValue = score;
            if (depth == MAX_ALPHA_BETA_DEPTH) {
               my_move = move;
            }
            if (score > alpha) {
               alpha = score;
            }
         }
      }
      else { // minimizing player 
         if (score < bestValue) {
            bestValue = score;
            if (depth == MAX_ALPHA_BETA_DEPTH) {
               my_move = move;
            }
            if (score < beta) {
               beta = score;
            }
         }
      }
      
      my_board.undo_move(move, board_status);
      
      if (beta <= alpha) {
         break; // alpha-beta pruning
      }
   }

   if (depth == MAX_ALPHA_BETA_DEPTH) {
      std::cout << "Total nodes searched: " << nodes_searched << std::endl;
      return 0;
   }
   
   return bestValue;
}


double Game::evaluate(){
	double evaluation = 0;
	return evaluation;
}

double Game::evaluate_piece(Piece* piece) {
    if (piece == NOPIECE) return 0.0;


    const char* pos = sq_to_coord(square);

    int x = pos[0] - 'a';

    int y = 8 - (pos[1] - '0');

    double p_evaluation = 0;


    
    static const double piece_values[] = {100, 300, 300, 500, 900, 9000};

    static const double pawn_table[8][8] = {
        {  0,   0,   0,   0,   0,   0,   0,   0 },
        {  5,   5,   5,   5,   5,   5,   5,   5 },
        {  1,   1,   2,   3,   3,   2,   1,   1 },
        {  0.5, 0.5, 1,   2.5, 2.5, 1,   0.5, 0.5 },
        {  0,   0,   0,   2,   2,   0,   0,   0 },
        {  0.5,-0.5,-1,   0,   0,-1,  -0.5, 0.5 },
        {  0.5, 1,   1,  -2,  -2,   1,   1,  0.5 },
        {  0,   0,   0,   0,   0,   0,   0,   0 }
    };

    static const double knight_table[8][8] = {
        {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0 },
        {-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0 },
        {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0 },
        {-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0 },
        {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0 },
        {-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0 },
        {-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0 },
        {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0 }
    };


    static const double bishop_table[8][8] = {
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 },
        {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0 },
        {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0 },
        {-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0 },
        {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0 },
        {-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0 },
        {-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0 },
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 }
    };


	static const double rook_table[8][8] = {
		{ 0.0,  0.0,  0.0,  0.5,  0.5,  0.0,  0.0,  0.0 },
		{ 0.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0 },
		{ 0.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0 },
		{ 0.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0 },
		{ 0.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0 },
		{ 0.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0 },
		{ 0.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0 },
		{ 0.0,  0.0,  0.0,  0.5,  0.5,  0.0,  0.0,  0.0 }
	};

	static const double queen_table[8][8] = {
		{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0 },
		{-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0 },
		{-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0 },
		{-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5 },
		{ 0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5 },
		{-1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0 },
		{-1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0 },
		{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0 }
	};

	static const double king_table[8][8] = {
		{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		{-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0 },
		{-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0 },
		{ 2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0 },
		{ 2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0 }
	};

    switch (piece) {
        case PAWN:
            p_evaluation += piece_values[0] + pawn_table[y][x];
            break;
        case pawn:
            p_evaluation -= piece_values[0] + pawn_table[7 - y][x];
            break;
        case KNIGHT:
            p_evaluation += piece_values[1] + knight_table[y][x];
            break;
        case knight:
            p_evaluation -= piece_values[1] + knight_table[7 - y][x];
            break;
        case BISHOP:
            p_evaluation += piece_values[2] + bishop_table[y][x];
            break;
        case bishop:
            p_evaluation -= piece_values[2] + bishop_table[7 - y][x];
            break;
        case ROOK:
            p_evaluation += piece_values[3] + rook_table[y][x];
            break;
        case rook:
            p_evaluation -= piece_values[3] + rook_table[7 - y][x];
            break;
        case QUEEN:
            p_evaluation += piece_values[4] + queen_table[y][x];
            break;
        case queen:
            p_evaluation -= piece_values[4] + queen_table[7 - y][x];
            break;
    }

	return p_evaluation;
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
