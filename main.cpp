#include <iomanip>
#include <iostream>

#include "Board.h"
#include "Definitions.h"
#include "Parser.h"

#define TESTING true
// TODO: all use same clang-format style
// TODO: investigate perft(7), maybe castling ?

int main(int argc, char **argv) {

#if TESTING
    // TODO: important: fix promotion correct lowercase output even if WHITE color
    //  TESTING: FEN positions
    std::vector<std::pair<std::string, int>> FENs = {
        FEN_POS_STARTING, FEN_POS_2, FEN_POS_3, FEN_POS_4, FEN_POS_4b, FEN_POS_CHECKCHECKCHECK, FEN_POS_5, FEN_POS_6};

    // FENs = {FEN_POS_5};

    for (const auto &FEN : FENs) {
        Board board = Board(FEN.first);
        std::cout << "Testing FEN: " << FEN.first << std::endl;
        std::cout << board.get_all_legal_moves() << board.get_all_legal_moves().size() << std::endl;
        std::cout << "Expected: " << FEN.second << std::endl;
    }

    Board default_board = Board();
    int max_depth = 5;
    std::vector<long long> reference = {0, 20, 400, 8902, 197281, 4865609, 119060324, 3195901860};
    std::cout << "\n[INITIAL_POS] Comparison with Perft Results from chessprogramming.org:" << std::endl;
    std::cout << "Depth\tCalculated\tError\tseconds\tKpos/s" << std::endl;
    for (size_t depth = 1; depth <= max_depth; depth++) {
        clock_t start = clock();
        long long positions = default_board.perft_search(depth);
        clock_t end = clock();
        double time_taken = double(end - start) / CLOCKS_PER_SEC;
        std::cout << depth << "\t" << positions << "\t\t" << positions - reference[depth] << "\t" << std::fixed
                  << std::setprecision(2) << time_taken << "\t" << (positions / 1e3 / time_taken) << std::endl;
    }
    std::cout << std::endl;

    default_board = Board(FEN_POS_2.first);
    max_depth = 4;
    reference = {0, 48, 2039, 97862, 4085603, 193690690, 8031647685};
    std::cout << "\n[FEN_POS_2] Comparison with Perft Results from chessprogramming.org:" << std::endl;
    std::cout << "Depth\tCalculated\tError\tseconds\tKpos/s" << std::endl;
    for (size_t depth = 1; depth <= max_depth; depth++) {
        clock_t start = clock();
        long long positions = default_board.perft_search(depth);
        clock_t end = clock();
        double time_taken = double(end - start) / CLOCKS_PER_SEC;
        std::cout << depth << "\t" << positions << "\t\t" << positions - reference[depth] << "\t" << std::fixed
                  << std::setprecision(2) << time_taken << "\t" << (positions / 1e3 / time_taken) << std::endl;
    }
    std::cout << std::endl;

    exit(0);
#endif

    // // NORMAL LOGIC : parsing history file
    // Parser parser{argc, argv};
    // parser.parseArgs();
    // Board board;
    // parser.parseHistory(board);
    // BitMoveVec moves = board.get_all_legal_moves();

    // std::cout << moves << std::endl;

    // BoardState state (board);
    // for (auto &move : moves) {
    //     board.move(move);
    //     std::cout << board << std::endl;
    //     state.apply(board);
    //     getchar();
    // }

    // Board fen_test_board = Board(FEN_POS_2);
    // std::cout << fen_test_board << std::endl;
    // std::cout << fen_test_board.get_all_legal_moves() << std::endl;

    // fen_test_board.print_attacked(1-fen_test_board.turn);

    // std::cout << Board(FEN_POS_STARTING).get_all_legal_moves() << 20 << std::endl;
    // std::cout << Board(FEN_POS_2).get_all_legal_moves() << 48 << std::endl;
    // std::cout << Board(FEN_POS_3).get_all_legal_moves() << 14 << std::endl; // king moves legalness
    // std::cout << Board(FEN_POS_4).get_all_legal_moves() << 6 << std::endl;

    // std::cout << "TURN FOR " << (turn ? "black" : "white") << std::endl;
    // std::vector<std::string> moves = Chess.my_board.all_legal_moves(turn);

    // // for (auto & element : moves) std::cout << element << std::endl;

    // // moves = Chess.my_board.all_legal_moves(1);
    // // for (auto&element:moves) std::cout << element << std::endl;
    // // std::cout << Chess.search(4,1) << std::endl;
    // int rand_int = rand() % moves.size();

    // // double db = Chess.search_best(MAX_ALPHA_BETA_DEPTH, turn, turn?-9999.0:9999.0);
    // double db = Chess.search_best_alpha_beta(MAX_ALPHA_BETA_DEPTH, turn, turn?-999999:999999, -999999, 999999);

    // std::cout << Chess.my_move << std::endl;
    // // std::cout << moves[rand_int] << std::endl;
    // // parser.writeNextMove(moves[rand_int]);
    // parser.writeNextMove(Chess.my_move);
    // Chess.my_board.display();

    // BitMoveVec moves = board.get_all_legal_moves();
    // std::cout << moves << std::endl;

    return 0;
}
