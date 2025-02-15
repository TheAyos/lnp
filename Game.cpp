#include "Game.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "BitMove.h"
#include "Board.h"
#include "Definitions.h"
#include "Evaluation.h"
#include "MoveOrdering.h"
#include "TT.h"

// FIXME: safety handling & find a better way of passing object instances
// FIXME: invalid move to init
Game::Game(Board& board, Openings* openings) : board(board), openings(openings), nSearchedNodes(0), globalBestMove(-1) {
}

/* -------------------------------------------------------------------------- */
/*                search and evaluation (i.e. actually playing)               */
/* -------------------------------------------------------------------------- */

// TOKEEP: for debugging purposes
void Game::search_random(U64& bestMove) {
    MoveList moves = board.get_all_legal_moves();
    // std::cout << "---------------[search_random]: moves: " << std::endl;
    // std::cout << "---------------[search_random]: moves: " << std::endl;
    // std::cout << board << moves;
    // std::cout << "[search_random]: moves---------------" << std::endl;
    // std::cout << "[search_random]: moves---------------" << std::endl;
    srand(time(0));
    if (!moves.size()) Util::exitError("search_random: no moves generated !!");
    bestMove = moves[rand() % moves.size()].get_bit_repr();
    // std::cout << "[search_random]: " << sq_to_coord(bestMove->get_from()) << ", " << sq_t'/home/siyuan-lu/csproject/logic-and-proofs/Game.cpp' o_coord(bestMove->get_to())
    //           << ", " << bestMove->get_algebraic_notation() << std::endl;
    std::cout << "[search_random]: " << BitMove(bestMove).get_algebraic_notation() << std::endl;
}

// int Game::search_best_minimax(U64& bestMove, int depth) {
//     if (depth == 0) return evaluate();

//     static long nodes_searched = 0;
//     if (depth == MAX_ALPHA_BETA_DEPTH) {
//         nodes_searched = 0;  // Reset counter at root
//     }
//     nodes_searched++;

//     int bestValue = (board.turn == W) ? -INFTY : INFTY;
//     BitMoveVec moves = board.get_all_legal_moves();

//     for (const auto& move : moves) {
//         BoardState savedState(board);
//         if (board.make_move(move) == -1) continue;

//         int score = search_best_minimax(bestMove, depth - 1);
//         savedState.reapply(board);

//         if (board.turn == W) {  // Maximizing player
//             if (score > bestValue) {
//                 bestValue = score;
//                 if (depth == MAX_ALPHA_BETA_DEPTH) {
//                     bestMove = move.get_bit_repr();
//                 }
//             }
//         } else {  // Minimizing player
//             if (score < bestValue) {
//                 bestValue = score;
//                 if (depth == MAX_ALPHA_BETA_DEPTH) {
//                     bestMove = move.get_bit_repr();
//                 }
//             }
//         }
//     }

//     if (depth == MAX_ALPHA_BETA_DEPTH) {
//         std::cout << "[MINIMAX]Total nodes searched: " << nodes_searched << std::endl;
//         return 0;
//     }

//     return bestValue;
// }

/*
_______
MVV_LVA
more valuable the captured piece is, and less valuable the attacker is, 
the stronger the capture will be
_______

    (Victims) Pawn Knight Bishop   Rook  Queen   King
  (Attackers)
        Pawn   105    205    305    405    505    605
      Knight   104    204    304    404    504    604
      Bishop   103    203    303    403    503    603
        Rook   102    202    302    402    502    602
       Queen   101    201    301    401    501    601
        King   100    200    300    400    500    600

*/
int MVV_LVA[12][12] = 
{
 	105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
	104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
	103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
	102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
	101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
	100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

	105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
	104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
	103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
	102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
	101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
	100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
};

//get move score from a move 
int Game::get_MVV_LVA_score(const BitMove& move) {
    int victim = board.get_piece_on_square(move.get_to());
    int attacker = move.get_piece();
    return (victim >= 0) ? MVV_LVA[victim][attacker] : 0; // if not capture = 0
}

void Game::sort_moves_by_MVV_LVA(BitMoveVec& moves) {
    std::sort(moves.begin(), moves.end(), [&](const BitMove& a, const BitMove& b) { // [&] = lambda notation
        return get_MVV_LVA_score(a) > get_MVV_LVA_score(b);
    });
}


U64 Game::search(std::chrono::time_point<std::chrono::high_resolution_clock> start) {
    auto end = start + std::chrono::seconds(MAX_SEARCH_TIME_S);  // time constraint

    nSearchedNodes = 0;
    globalBestMove = 0;
    int score = 0;

    // iterative deepening
    for (int depth = 1; depth <= MAX_SEARCH_DEPTH; depth++) {
        auto now = std::chrono::high_resolution_clock::now();
        if (now >= end) {
            if (DEBUG) std::cout << "[GAME::SEARCH] Time ! stopping search at depth " << depth - 1 << std::endl;
            break;
        }

        U64 localBestMove = 0;
        score = search_best_alpha_beta(localBestMove, depth, depth, -INFTY, INFTY, end);
        now = std::chrono::high_resolution_clock::now();
        // search done in time, can set new best move
        if (now < end) {
            // std::cout << "done search in time, setting new best " << BitMove(localBestMove).get_algebraic_notation()
            // << std::endl;
            globalBestMove = localBestMove;
        }

        if (globalBestMove == 0) {
            if (DEBUG) std::cout << "[GAME::SEARCH] no move found, fallback" << std::endl;
            // search_random(globalBestMove);
            globalBestMove = board.get_all_legal_moves()[0].get_bit_repr();
        }

        now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        std::cout << "info score " << score << " depth " << depth << " nodes " << nSearchedNodes << " time " << duration
                  << std::endl;
    }

    return globalBestMove;
}

int Game::search_best_alpha_beta(U64& bestMove,
                                 int initial_depth,
                                 int depth,
                                 int alpha,
                                 int beta,
                                 std::chrono::time_point<std::chrono::high_resolution_clock> end) {
    auto now = std::chrono::high_resolution_clock::now();
    if (now >= end) {
        if (DEBUG) std::cout << "[GAME::SEARCH_A_B] Time ! return from search at depth " << depth << std::endl;
        return (board.turn == W) ? -INFTY : INFTY;  // time limit is reached, do not update bestMove
    }
    // FIXME: testing for improvement, still need to understand and fix quiescence_search
    // if (depth == 0) return quiescence_search(bestMove, initial_depth, depth, alpha, beta, end);
    if (depth == 0) return Evaluation::eval(board);

    nSearchedNodes++;

    int bestValue = (board.turn == W) ? -INFTY : INFTY;
    int score = 0;

    MoveList moves = board.get_all_legal_moves();

    /* ------------------------------ move ordering ----------------------------- */
    // move last best move to the front
    /* if (initial_depth == depth && globalBestMove != 0) {
        auto it = std::find_if(
            moves.begin(), moves.end(), [&](const BitMove& move) { return move.get_bit_repr() == globalBestMove; });
        if (it != moves.end()) {
            std::iter_swap(moves.begin(), it);
        }
    }*/
    MoveOrdering::MVV_LVA(board, moves);

    for (const auto& move : moves) {
        BoardState savedState(board);
        board.make_move(move);
        score = search_best_alpha_beta(bestMove, initial_depth, depth - 1, alpha, beta, end);
        if (now >= end) return (board.turn == W) ? -INFTY : INFTY;  // time limit is reached, return worst score to skip
        savedState.reapply(board);

        if (board.turn == W) {  // maximizing player
            if (score > bestValue) {
                bestValue = score;
                if (depth == initial_depth) {
                    bestMove = move.get_bit_repr();
                }
                if (score > alpha) {
                    alpha = score;
                }
            }
        } else {  // minimizing player
            if (score < bestValue) {
                bestValue = score;
                if (depth == initial_depth) {
                    bestMove = move.get_bit_repr();
                }
                if (score < beta) {
                    beta = score;
                }
            }
        }

        if (beta <= alpha) break;  // alpha-beta pruning
    }

    if (depth == initial_depth) {
        if (DEBUG) {
            std::cout << "searched and found best move in search: " << BitMove(bestMove).get_algebraic_notation()
                      << std::endl;
            std::cout << "Total nodes searched: " << nSearchedNodes << std::endl;
        }
        // return 0; // problematic ???
    }

    return bestValue;
}

int Game::quiescence_search(U64& bestMove,
                            int initial_depth,
                            int depth,
                            int alpha,
                            int beta,
                            std::chrono::time_point<std::chrono::high_resolution_clock> end) {
    auto now = std::chrono::high_resolution_clock::now();
    if (now >= end) {
        if (DEBUG) std::cout << "[GAME::QUIESCE] Time ! return" << std::endl;
        return (board.turn == W) ? -INFTY : INFTY;  // time limit is reached, do not update bestMove
    }
    // CRITICAL? : allow for "evasion moves" when in check ? but too slow ??
    // if (board.playerInCheck(board.turn))
    //     return search_best_alpha_beta(bestMove, initial_depth, depth, alpha, beta, end);

    // int eval = evaluate();
    int eval = Evaluation::eval(board);
    if (eval >= beta) return beta;
    if (alpha < eval) alpha = eval;

    MoveList captures = board.get_capture_moves();
    MoveOrdering::MVV_LVA(board, captures);
    // MoveOrdering::PV_P(board, pv, moves);

    for (auto& move : captures) {
        BoardState savedState(board);
        board.make_move(move);
        int score = -quiescence_search(bestMove, initial_depth, depth, -beta, -alpha, end);
        savedState.reapply(board);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    return alpha;
}

// get opening move
std::string Game::playOpeningMove(int argc, char** argv) {
    std::string bestMoveStr = openings->getMove(board);
    if (!bestMoveStr.empty()) {
        // transform a move in alg notation (ex : e2e4) to a bitmove
        Parser parser(board, argc, argv);
        BitMove bestMove = parser.parse_algebraic_move(
            coord_to_sq(bestMoveStr.substr(0, 2)), coord_to_sq(bestMoveStr.substr(2, 2)), ' ');
        // check legality of opening move
        std::cout << bestMoveStr << std::endl;
        std::cout << bestMoveStr << std::endl;
        std::cout << bestMoveStr << std::endl;
        board.make_move(bestMove);
        return bestMove.get_algebraic_notation();
    }
    return "";
    // else no opening move, proceed with normal search
}
