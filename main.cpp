#include <iostream>  // IWYU pragma: keep

#include "Board.h"  // IWYU pragma: keep
#include "Game.h"  // IWYU pragma: keep
#include "Parser.h"  // IWYU pragma: keep
#include "testing/PerftTests.h"  // IWYU pragma: keep

// TODO: all use same clang-format style

// IMPORTANT: /!\ make sure /!\ to set to false before pushing to main
#define TESTING true

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

    // NORMAL LOGIC : parsing history file
    Board board;
    Game game(board);

    Parser parser{board, argc, argv};
    parser.parseArgs();
    parser.parseHistory();

    BitMoveVec moves = board.get_all_legal_moves();
    // std::cout << board << std::endl;
    std::cout << "bestMove ptr before search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "bestMove ptr before search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "bestMove ptr before search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "bestMove ptr before search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    game.search_random();
    // game.search_negamax_alpha_beta(7, 99999, -99999);
    // game.search_best_alpha_beta(MAX_ALPHA_BETA_DEPTH, 0, 99999, -99999);
    std::cout << "bestMove ptr after search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "bestMove ptr after search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "bestMove ptr after search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "bestMove ptr after search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "bestMove ptr after search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "bestMove ptr after search: " << game.bestMove << "value: " << game.bestMove->get_algebraic_notation()
              << std::endl;
    std::cout << "how i see the board before writing next move: " << board;
    parser.writeNextMove(game.bestMove->get_algebraic_notation());

    // TODO: check correct parser output
    // TODO: crucial: check that we always have a move to play
    // int rand_int = rand() % moves.size();
    // // std::cout << moves[rand_int] << std::endl;
    // // parser.writeNextMove(moves[rand_int]);
    // Chess.my_board.display();

    return 0;
}
#endif