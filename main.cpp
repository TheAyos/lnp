#include <iostream>  // IWYU pragma: keep
#include <cassert>

#include "Board.h"  // IWYU pragma: keep
#include "Definitions.h"  // IWYU pragma: keep
#include "Game.h"  // IWYU pragma: keep
#include "Parser.h"  // IWYU pragma: keep
#include "testing/PerftTests.h"  // IWYU pragma: keep
#include "Openings.h"

// TODO: all use same clang-format style

// IMPORTANT: /!\ make sure /!\ to set to false before pushing to main
#define TESTING false

#if TESTING
int main(int argc, char **argv) {
    // Board board;
    // Openings openings = Openings("openings.txt");
    // Game game(board, &openings);
    
    // if (game.board.ply <= 10) {
    //     std::string move = game.playOpeningMove(argc, argv);
    //     std::cout << move << std::endl;
    // }
    // std::cout << openings.getMove(board) << std::endl;

    run_perft_tests();

    // DEBUGGING: to debug move generation in details at higher depths
    // // inspired by https://github.com/agausmann/perftree to find bugs in movegen, comparing with stockfish
    // Board perftree_board = Board(FEN_POS_STARTING.first);
    // std::cout << perftree_board;
    // perftree_board.perftree(7);

    // TESTING: visualize moves one by one
    // BoardState state (board);
    // for (auto &move : moves) {
    //     board.move(move);
    //     std::cout << board << std::endl;
    //     state.reapply(board);
    //     std::cin.get();
    // }
    return 0;
}
#else
int main(int argc, char **argv) {

    Board board;
    Openings openings("openings.txt");
    Game game(board, &openings);


    // TOKEEP: for Visual Studio debugging
    // int fargc = 5;
    // char *fargv[] = {"logic-and-proofs", "-H", "../../../../lnp/history_ex_1.txt", "-m", "move.txt"};
    // Parser parser{board, fargc, fargv};

    // NORMAL LOGIC : parsing history file
    Parser parser{board, argc, argv};
    parser.parseArgs();
    parser.parseHistory();
    
    //TODO: integrate this to the rest of the search logic
    if (board.ply <= 20) {
        std::cout << "Opening move :" << std::endl;
        std::string move = game.playOpeningMove(argc, argv);
        if (!move.empty()) {
            parser.writeNextMove(move);
            return 0;
        }
    }
    
    // BitMoveVec moves = board.get_all_legal_moves();
    // NICEDEBUG // std::cout << board << moves << std::endl;

    U64 bestMove = -1;

    std::cout << "starting search..." << std::endl;

    // to avoid cases where bestMove stays = -1 after alpha-beta search (when all possible moves have negative scores)
    
    game.search_random(bestMove);
    // int score = game.search_best_minimax(bestMove, MAX_ALPHA_BETA_DEPTH);
    int score = game.search_best_alpha_beta(bestMove, MAX_ALPHA_BETA_DEPTH, -99999, 99999);
    // int score = game.search_negamax_alpha_beta(bestMove, MAX_ALPHA_BETA_DEPTH, -99999, 99999);
    
    std::cout << "info score depth " << MAX_ALPHA_BETA_DEPTH << " score " << score << std::endl;
    if (bestMove == -1) Util::exitError("no move found here in main !!");

    // std::cout << "bestMove ptr after search: " << bestMove << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << "value: " << bestMove->get_algebraic_notation()
    //           << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << "value: " << bestMove->get_algebraic_notation()
    //           << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << "value: " << bestMove->get_algebraic_notation()
    //           << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << "value: " << bestMove->get_algebraic_notation()
    //           << std::endl;
    // std::cout << "bestMove ptr after search: " << bestMove << "value: " << bestMove->get_algebraic_notation()
    //           << std::endl;
    std::cout << "how i see the board before writing next move: " << board;
    parser.writeNextMove(BitMove(bestMove).get_algebraic_notation());

    return 0;
}
#endif
