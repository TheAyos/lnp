#include <cassert>
#include <iostream>  // IWYU pragma: keep

#include "Board.h"  // IWYU pragma: keep
#include "Definitions.h"  // IWYU pragma: keep
// #include "Game.h"  // IWYU pragma: keep
#include "Openings.h"
#include "Parser.h"  // IWYU pragma: keep
#include "testing/PerftTests.h"  // IWYU pragma: keep
#include "Search.h"
#include "TT.h"

// TODO: all use same clang-format style

// BEFORE PUSHING TO MAIN
// /!\ important to change before committing to main
// -> Game.h constants and debug mode
// /!\ make sure /!\ to set to false below
#define TESTING false

#if TESTING
int main(int argc, char **argv) {
    //TESTING openings
    /* Board board;
    Openings openings = Openings("openings.txt");
    Game game(board, &openings);

    if (game.board.ply <= 10) {
        std::string move = game.playOpeningMove(argc, argv);
        std::cout << move << std::endl;
    }
    std::cout << openings.getMove(board) << std::endl;*/

    // TESING performance
    /*run_perft_tests();*/

    // DEBUGGING: to debug move generation in details at higher depths
    // inspired by https://github.com/agausmann/perftree to find bugs in movegen, comparing with stockfish
    // TT::init(16);
    Board board = Board("5B2/6P1/1p6/8/1N6/kP6/2K5/8 w - - ");
    MoveList moves = board.get_all_legal_moves();
    moves.finish();
    
    std::cout << "yep" << std::endl; 
    for (auto& move: moves) {
	move.print();
    	// move.print();
    }
    std::cout << "done" << std::endl;

    MoveOrdering::MVV_LVA(board, moves);

    for (auto& move: moves) {
	move.print();
    	// move.print();
    }

    // std::cout << perftree_board;
    // perftree_board.perftree(6);
    
    std::cout << "Hello" << std::endl;
    // TESTING: visualize moves one by one
    /*BoardState state (board);
    for (auto &move : moves) {
        board.move(move);
        std::cout << board << std::endl;
        state.reapply(board);
        std::cin.get();
    }*/
    return 0;
}
#else
int main(int argc, char **argv) {

    auto start = std::chrono::high_resolution_clock::now();

    // Board board ("r1bqkbnr/pppn1ppp/3p4/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R w KQkq ");// should randomly play f1c4 or b1c3
    // (from openings.txt)
    TT::init(16);
    Board board;
    Openings openings("openings.txt");
    // Game game(board, &openings);

    // TOKEEP: for Visual Studio debugging
    // int fargc = 5;
    // char *fargv[] = {"logic-and-proofs", "-H", "../../../../lnp/history_ex_1.txt", "-m", "move.txt"};
    // Parser parser{board, fargc, fargv};

    // NORMAL LOGIC : parsing history file
    Parser parser{board, argc, argv};
    parser.parseArgs();
    parser.parseHistory();
    
    std::cout << "how i see the board before writing next move: " << board;

    /* 
    // TODO: integrate this to the rest of the search logic
    if (board.ply <= 20) {
        std::string moveAlgebraicRepr = game.playOpeningMove(argc, argv);
        std::cout << "Opening move :" << moveAlgebraicRepr << std::endl;
        if (!moveAlgebraicRepr.empty()) {
            parser.writeNextMove(moveAlgebraicRepr);
            return 0;
        }
    } */

    std::cout << "starting search..." << std::endl;
    BitMove bestMove = Search::BestMoveTime(board, 9500);
    // U64 bestMove = game.search(start); 
    // std::cout << "current hash" << board.get_hash() << std::endl; 
    // std::cout << board << std::endl;

    // if (!bestMove) Util::exitError("no move found here in main !!");
    // parser.writeNextMove(BitMove(bestMove).get_algebraic_notation());
    parser.writeNextMove(bestMove.get_algebraic_notation());

    return 0;
}
#endif
