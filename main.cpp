#include <stdlib.h>

#include <iostream>

#include "Board.h"
#include "Definitions.h"

#define TESTING false

// TODO: all use same clang-format style
int main(int argc, char **argv) {

#if TESTING
    {
        // v<1.0 - at depth 5, only en passant are missing, but more at depth 6 ?
        // Depth   Calculated      Error   seconds Kpos/s
        // 1       20              0       2.6e-05 7.7
        // 2       400             0       0.0003  13
        // 3       8902            0       0.0065  14
        // 4       197281          0       0.16    13
        // 5       4865351         -258    3.9     13
        // 6       119048441       -11883  1e+02   12
        // v1.0 - after adding special moves:
        // Depth   Calculated      Error   seconds Kpos/s
        // 1       20              0       6.5e-05 3.1
        // 2       400             0       0.00088 4.6
        // 3       8902            0       0.018   4.8
        // 4       197281          0       0.43    4.6
        // 5       4865609         0       11      4.5
        // 6       119060324       0       2.8e+02 4.3
        // TESTING: perft results
        int max_depth = 7;
        std::vector<long> reference = {0, 20, 400, 8902, 197281, 4865609, 119060324, 3195901860};
        std::cout << "\nComparison with Perft Results from chessprogramming.org:" << std::endl;
        std::cout << "Depth\tCalculated\tError\tseconds\tKpos/s" << std::endl;
        for (int depth = 1; depth <= max_depth; depth++) {
            clock_t start = clock();
            int positions = Chess.search(depth, 1);
            clock_t end = clock();
            double time_taken = double(end - start) / CLOCKS_PER_SEC;
            std::cout << depth << "\t" << positions << "\t\t" << positions - reference[depth] << "\t"
                      << std::setprecision(2) << time_taken << "\t" << (positions / 10e4 / time_taken) << std::endl;
        }
        std::cout << std::endl;
        exit(0);
    }
#endif

    // Parser parser{argc, argv};
    // parser.parseArgs();
    // Board board;
    // parser.parseHistory(board);
    // std::cout << board.get_all_legal_moves() << std::endl;

    std::vector<std::pair<std::string, int>> FENs = {
        {FEN_POS_STARTING, 20}, 
        {FEN_POS_2, 48}, 
        {FEN_POS_3, 14}, 
        {FEN_POS_4, 6}
    };
    for (const auto &FEN : FENs) {
        Board board = Board(FEN.first);
        std::cout << board << board.get_all_legal_moves();
        std::cout << "Expected: " << FEN.second << std::endl;
    }

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
