#pragma once
#include "BitMove.h"
#include "Board.h"
#define MAX_ALPHA_BETA_DEPTH 5

class Game {
   public:
    Board& board;

    Game(Board& board);

    /* -------------------------- search and evaluation ------------------------- */
    int evaluate();
    int evaluate_piece(int piece, int rank, int file);
    void search_random(int& bestMove);
    int search_best_alpha_beta(int& bestMove, int depth, int prev_eval, int alpha, int beta);
    // int search_negamax_alpha_beta(BitMove* bestMove, int depth, int alpha, int beta);
};
