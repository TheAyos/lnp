#pragma once
#include "BitMove.h"
#include "Board.h"
#include "Openings.h"
#include "Parser.h"

#include <chrono>

#define MAX_ALPHA_BETA_DEPTH 5 // legacy

// /!\ important to change before committing to main
#define DEBUG false
#if DEBUG
    #define MAX_SEARCH_DEPTH 100
    #define MAX_SEARCH_TIME_S 3
#else
    #define MAX_SEARCH_DEPTH 100
    #define MAX_SEARCH_TIME_S 9
#endif

class Game {
   public:
    Board& board;
    U64 globalBestMove;
    Openings *openings;

    Game(Board& board, Openings *openings);

    long nSearchedNodes;

    /* -------------------------- search and evaluation ------------------------- */
    int evaluate();
    int evaluate_piece(int piece, int rank, int file);
    void search_random(U64& bestMove);
    // int search_best_minimax(U64& bestMove, int depth);

    std::string playOpeningMove(int argc, char **argv);

    U64 search(std::chrono::time_point<std::chrono::high_resolution_clock> start);
    int search_best_alpha_beta(U64& bestMove, int initial_depth, int depth, int alpha, int beta, std::chrono::time_point<std::chrono::high_resolution_clock> end);
    // avoid horizon effect, first parameters are only for fallback on alpha beta
    int quiescence_search(U64& bestMove, int initial_depth, int depth, int alpha, int beta, std::chrono::time_point<std::chrono::high_resolution_clock> end);
};
