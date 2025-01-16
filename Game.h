#include "Board.h"
#include "Openings.h"
#include "Parser.h"
#define MAX_ALPHA_BETA_DEPTH 5

class Game {
   public:
    Board& board;
    BitMove *bestMove;
    Openings *openings;

    Game(Board& board, Openings *openings);

    /* -------------------------- search and evaluation ------------------------- */
    int evaluate();
    int evaluate_piece(int piece, int square);
    void search_random(int& bestMove);
    int search_best_alpha_beta(int& bestMove, int depth, int prev_eval, int alpha, int beta);
    // int search_negamax_alpha_beta(BitMove* bestMove, int depth, int alpha, int beta);
    std::string playOpeningMove(int argc, char **argv);
};
