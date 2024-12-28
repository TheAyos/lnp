#pragma once
#include "BitMove.h"
#include "Board.h"
#define MAX_ALPHA_BETA_DEPTH 4

class Game {
   public:
    Board &board;
    BitMove *bestMove;

    Game(Board &board);
    ~Game();

    /* -------------------------- search and evaluation ------------------------- */
    int evaluate();
    int evaluate_piece(int piece);
    void search_random();
    int search_best_alpha_beta(int depth, int prev_eval, int alpha, int beta);
    int search_negamax_alpha_beta(int depth, int alpha, int beta);
};
