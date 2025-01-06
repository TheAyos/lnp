#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "BitMove.h"
#include "BitOps.h"
#include "Board.h"
#include "Definitions.h"

// FIXME: safety handling & find a better way of passing object instances
// FIXME: invalid move to init
Game::Game(Board& board) : board(board) {
}

/* -------------------------------------------------------------------------- */
/*                search and evaluation (i.e. actually playing)               */
/* -------------------------------------------------------------------------- */

// TOKEEP: for debugging purposes
void Game::search_random(int& bestMove) {
    BitMoveVec moves = board.get_all_legal_moves();
    // std::cout << "---------------[search_random]: moves: " << std::endl;
    // std::cout << "---------------[search_random]: moves: " << std::endl;
    // std::cout << board << moves;
    // std::cout << "[search_random]: moves---------------" << std::endl;
    // std::cout << "[search_random]: moves---------------" << std::endl;
    srand(time(0));
    if (!moves.size()) Util::exitError("search_random: no moves generated !!");
    bestMove = moves[rand() % moves.size()].get_bit_repr();
    // std::cout << "[search_random]: " << sq_to_coord(bestMove->get_from()) << ", " << sq_to_coord(bestMove->get_to())
    //           << ", " << bestMove->get_algebraic_notation() << std::endl;
    std::cout << "[search_random]: " << BitMove(bestMove).get_algebraic_notation() << std::endl;
}

// WIP
// int Game::search_best_alpha_beta(int& bestMove, int depth, int alpha, int beta) {
//     // alpha is the best value that the maximizing player currently can guarantee
//     // beta is the best value that the minimizing player currently can guarantee
//     if (depth == 0) return evaluate();
//     static long nodes_searched = 0;
//     static int ply = 0;
//     if (depth == MAX_ALPHA_BETA_DEPTH) nodes_searched = 0;
//     nodes_searched++;

//     int prevAlpha = alpha;
//     BitMoveVec moves = board.get_all_legal_moves();
//     int bestLocalMove = -1;

//     for (BitMove& mv : moves) {

//         BoardState savedState = BoardState(board);
//         board.move(mv);
//         int score = -search_best_alpha_beta(bestMove, depth - 1, -beta, -alpha);
//         savedState.reapply(board);  // restore board state
//         if (score >= beta) {
//             return beta;
//         }
//         if (score > alpha) {
//             alpha = score;
//             if (depth == 0) bestLocalMove = mv.get_bit_repr();
//         }
//     }

//     if (alpha != prevAlpha) {
//         bestMove = bestLocalMove;  // BRUH
//         std::cout << "Best move in search: " << BitMove(bestLocalMove).get_algebraic_notation() << std::endl;
//         std::cout << "Total nodes searched: " << nodes_searched << std::endl;
//     } else {
//         std::cout << "No moves found" << std::endl;
//     }
//     return alpha;
// }

int Game::search_best_alpha_beta(int& bestMove, int depth, int prev_eval, int alpha, int beta) {
    if (depth == 0) return evaluate();

    static long nodes_searched;
    if (depth == MAX_ALPHA_BETA_DEPTH) {
        nodes_searched = 0;  // reset at start of new search
    }
    nodes_searched++;

    int bestValue = (board.turn == W) ? -99999 : 99999;
    int score = 0;

    BitMoveVec moves = board.get_all_legal_moves();
    for (const auto& move : moves) {

        BoardState savedState(board);
        board.move(move);
        score = search_best_alpha_beta(bestMove, depth - 1, prev_eval, alpha, beta);
        savedState.reapply(board);

        if (board.turn == W) {  // maximizing player
            // std::cout << "Whereeee" << std::endl;
            if (score > bestValue) {
                bestValue = score;
                if (depth == MAX_ALPHA_BETA_DEPTH) {
                    bestMove = move.get_bit_repr();
                    // std::cout << "try set bestmove(max): " << move.get_bit_repr() << std::endl;
                }
                if (score > alpha) {
                    alpha = score;
                }
            }
        } else {  // minimizing player
            // std::cout << "Bhereeee" << std::endl;
            if (score < bestValue) {
                bestValue = score;
                if (depth == MAX_ALPHA_BETA_DEPTH) {
                    bestMove = move.get_bit_repr();
                    // std::cout << "try set bestmove(min): " << move.get_bit_repr() << std::endl;
                }
                if (score < beta) {
                    beta = score;
                }
            }
        }

        if (beta <= alpha) break;  // alpha-beta pruning
    }

    if (depth == MAX_ALPHA_BETA_DEPTH) {
        std::cout << "searched and found best move in search: " << bestMove << std::endl;
        std::cout << "Total nodes searched: " << nodes_searched << std::endl;
        return 0;
    }

    return bestValue;
}

// WIP
/*int Game::search_negamax_alpha_beta(int depth, int alpha, int beta) {
    if (depth == 0) return evaluate();

    BitMoveVec moves = board.get_all_legal_moves();

    if (moves.size() == 0) {
        // TODO: need a board.player_in_check() function
        // if current player in check
        if (board.is_attacked(board.find_king(board.turn), 1 - board.turn))
            return -99999;
        else
            return 0; // stalemate
    }

    int bestScore = -99999;
    // FIXME: no new !!!
    BitMove bestLocalMove(h1, h1, -1, NO_PROMOTION, false, false, false, false);

    for (const BitMove& move : moves) {
        BoardState savedState = BoardState(board);
        board.move(move);
        // best for minimizing player is worst for maximizing player and vice versa (hence the alpha:=-beta,
beta:=-alpha) int score = -search_negamax_alpha_beta(depth - 1, -beta, -alpha); savedState.reapply(board);

        bestScore = std::max(bestScore, score);
        bestLocalMove = move;

        if (score >= beta) {
            bestLocalMove = move;
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            bestLocalMove = move;
        }
    }

    bestMove = new BitMove(bestLocalMove);
    return alpha;
}*/

int Game::evaluate() {
    int evaluation = 0;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            evaluation += evaluate_piece(board.get_piece_on_square(BitOps::rf_to_square(i, j)));

    // TODO: crucial: use justCheckCheck on board.move()
    // if black checks white
    if (board.is_attacked(board.find_king(W), B)) {
        evaluation -= 30;
        BitMoveVec moves = board.get_all_legal_moves();
        if (moves.size() == 0) return -99999;

    } else if (board.is_attacked(board.find_king(B), W)) {  // if white checks black
        evaluation += 30;
        BitMoveVec moves = board.get_all_legal_moves();
        if (moves.size() == 0) return 99999;
    }
    return evaluation;
}

int Game::evaluate_piece(int piece) {
    if (piece == -1) return 0;
    int p_evaluation = 0;
    if (piece == PAWN || piece == pawn) p_evaluation += 100;
    if (piece == KNIGHT || piece == knight || piece == BISHOP || piece == bishop) p_evaluation += 300;
    if (piece == ROOK || piece == rook) p_evaluation += 500;
    if (piece == QUEEN || piece == queen) p_evaluation += 900;
    if (piece == KING || piece == king) p_evaluation += 9000;
    // if piece white, positive evaluation, else negative
    return ((PAWN <= piece && piece <= KING) ? p_evaluation : -p_evaluation);
}