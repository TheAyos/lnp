#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../Board.h"

/* -------------------------------------------------------------------------- */
/*           source : https://www.chessprogramming.org/Perft_Results          */
/* -------------------------------------------------------------------------- */

// clang-format off
const std::string FEN_POS_STARTING = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string FEN_POS_2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
const std::string FEN_POS_3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
const std::string FEN_POS_4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
const std::string FEN_POS_4b = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ";
const std::string FEN_POS_CHECKCHECKCHECK = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2";
const std::string FEN_POS_5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ";
const std::string FEN_POS_6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ";

// commented out the depths that took too long to complete
const std::vector<std::pair<std::string, std::vector<long long>>> test_cases = {
    {FEN_POS_STARTING, {0, 20, 400, 8902, 197281, 4865609, 119060324}},  //, 3195901860, 84998978956, 2439530234167}},
    {FEN_POS_2, {0, 48, 2039, 97862, 4085603, 193690690}},  //, 8031647685}},
    {FEN_POS_3, {0, 14, 191, 2812, 43238, 674624, 11030083, 178633661}},  //, 3009794393}},
    {FEN_POS_4, {0, 6, 264, 9467, 422333, 15833292}},  //, 706045033}},
    {FEN_POS_4b, {0, 6, 264, 9467, 422333, 15833292}},  //, 706045033}},
    {FEN_POS_5, {0, 44, 1486, 62379, 2103487, 89941194}},
    {FEN_POS_6, {0, 46, 2079, 89890, 3894594, 164075551}}  //, 6923051137, 287188994746, 11923589843526, 490154852788714}},
};

// recommended to run to validate updates to move generation
// with the commented out cases, should take around 1min to complete
inline void run_perft_tests() {


    for (const auto& test_case : test_cases) {
        Board board = test_case.first.empty() ? Board() : Board(test_case.first);
        const auto& reference = test_case.second;
        int max_depth = reference.size() - 1;

        std::cout << "\n[" << test_case.first
                  << "] Comparison with Perft Results from chessprogramming.org:" << std::endl;
        std::cout << std::setw(5) << "Depth" << std::setw(15) << "Calculated" << std::setw(12) 
                  << "Error" << std::setw(10)
                  << "Seconds" << std::setw(10) << "Kpos/s" << std::endl;

        for (size_t depth = 1; depth <= max_depth; depth++) {
            clock_t start = clock();
            long long positions = board.perft_search(depth);
            clock_t end = clock();
            double time_taken = double(end - start) / CLOCKS_PER_SEC;
            std::cout << std::setw(5) << depth << std::setw(15) << positions << std::setw(12)
                      << positions - reference[depth] << std::setw(10) << std::fixed << std::setprecision(2)
                      << time_taken << std::setw(10) << (positions / 1e3 / time_taken) << std::endl;
        }
        std::cout << std::endl;
    }
}

/** RESULTS: LAST TIME RAN : 29/12/2024 by ayoub on commit after 8243ce4adb970cf64399c19f3f9a4136f08e3882

[rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1] Comparison with Perft Results from chessprogramming.org:
Depth   Calculated      Error   seconds Kpos/s
1       20              0       0.00    10000.00
2       400             0       0.00    26666.67
3       8902            0       0.00    29673.33
4       197281          0       0.01    29559.63
5       4865609         0       0.17    28225.41
6       119060324       0       4.23    28151.29
7       3195901860      0       116.56  27418.74


[r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ] Comparison with Perft Results from chessprogramming.org:
Depth   Calculated      Error   seconds Kpos/s
1       48              0       0.00    12000.00
2       2039            0       0.00    23170.45
3       97862           0       0.00    29065.04
4       4085603         0       0.15    28037.93
5       193690690       0       6.73    28775.14
6       8031647685      0       284.23  28257.69


[8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ] Comparison with Perft Results from chessprogramming.org:
Depth   Calculated      Error   seconds Kpos/s
1       14              0       0.00    7000.00
2       191             0       0.00    13642.86
3       2812            0       0.00    18025.64
4       43238           0       0.00    17735.03
5       674624          0       0.03    19319.68
6       11030083        0       0.61    17943.99
7       178633661       0       9.34    19135.55
8       3009794393      0       170.89  17612.35


[r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1] Comparison with Perft Results from chessprogramming.org:
Depth   Calculated      Error   seconds Kpos/s
1       6               0       0.00    2000.00
2       264             0       0.00    14666.67
3       9467            0       0.00    24978.89
4       422333          0       0.02    25647.23
5       15833292        0       0.60    26247.74
6       706045033       0       27.76   25436.28


[r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ] Comparison with Perft Results from chessprogramming.org:
Depth   Calculated      Error   seconds Kpos/s
1       6               0       0.00    2000.00
2       264             0       0.00    16500.00
3       9467            0       0.00    25517.52
4       422333          0       0.02    24432.08
5       15833292        0       0.60    26537.34
6       706045033       0       28.00   25219.53


[rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ] Comparison with Perft Results from chessprogramming.org:
Depth   Calculated      Error   seconds Kpos/s
1       44              0       0.00    8800.00
2       1486            0       0.00    24766.67
3       62379           0       0.00    23991.92
4       2103487         0       0.08    26476.27
5       89941194        0       3.79    23751.22


[r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ] Comparison with Perft Results from chessprogramming.org:
Depth   Calculated      Error   seconds Kpos/s
1       46              0       0.00    11500.00
2       2079            0       0.00    28094.59
3       89890           0       0.00    29973.32
4       3894594         0       0.13    30653.30
5       164075551       0       5.50    29835.74
6       6923051137      0       229.47  30169.89

*/