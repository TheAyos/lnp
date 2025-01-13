#include <iostream>  // IWYU pragma: keep
#include <cassert>

#include "Board.h"  // IWYU pragma: keep
#include "Definitions.h"  // IWYU pragma: keep
#include "Game.h"  // IWYU pragma: keep
#include "Parser.h"  // IWYU pragma: keep
#include "testing/PerftTests.h"  // IWYU pragma: keep

// TODO: all use same clang-format style

// IMPORTANT: /!\ make sure /!\ to set to false before pushing to main
#define TESTING false

#if TESTING
int main(int argc, char **argv) {
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
    Game game(board); 

    // TOKEEP: for Visual Studio debugging
    // int fargc = 5;
    // char *fargv[] = {"logic-and-proofs", "-H", "../../../../lnp/history_ex_1.txt", "-m", "move.txt"};
    // Parser parser{board, fargc, fargv};

    // NORMAL LOGIC : parsing history file
    Parser parser{board, argc, argv};
    parser.parseArgs();
    parser.parseHistory();
    
    BitMoveVec moves = board.get_all_legal_moves();
    // NICEDEBUG // std::cout << board << moves << std::endl;
    int bestMove = -1;
    std::cout << "starting search..." << std::endl;
    // game.search_random(bestMove);
    // game.search_best_alpha_beta(bestMove, MAX_ALPHA_BETA_DEPTH, -99999, 99999);
    game.search_best_alpha_beta(bestMove, MAX_ALPHA_BETA_DEPTH, (board.turn == W) ? -99999 : 99999, -99999, 99999);
    if (bestMove == -1) Util::exitError("no move found here in main !!");
    // game.search_negamax_alpha_beta(MAX_ALPHA_BETA_DEPTH, 0, 99999, -99999);
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

    // TODO: check correct parser output
    // TODO: crucial: check that we always have a move to play
    // int rand_int = rand() % moves.size();
    // // std::cout << moves[rand_int] << std::endl;
    // // parser.writeNextMove(moves[rand_int]);
    // Chess.my_board.display();

    return 0;
}
#endif
