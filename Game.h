#pragma once
#include "BitMove.h"
#include "Board.h"
#include "Openings.h"
#include "Parser.h"
#define MAX_ALPHA_BETA_DEPTH 5

class Game {
   public:
    Board& board;
    U64 bestMove;
    Openings *openings;

    Game(Board& board, Openings *openings);

    long nSearchedNodes;

    /* -------------------------- search and evaluation ------------------------- */
    int evaluate();
    int evaluate_piece(int piece, int rank, int file);
    void search_random(U64& bestMove);
    int search_best_minimax(U64& bestMove, int depth);
    int search_negamax_alpha_beta(U64& bestMove, int depth, int alpha, int beta);

    std::string playOpeningMove(int argc, char **argv);

    U64 search();
    int search_best_alpha_beta(U64& bestMove, int depth, int alpha, int beta);
    // avoid horizon effect
    int quiescence_search(int alpha, int beta);
};
