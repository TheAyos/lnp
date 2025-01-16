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
Game::Game(Board& board, Openings* openings) : board(board), openings(openings), nSearchedNodes(0), bestMove(-1) {
}

/* -------------------------------------------------------------------------- */
/*                search and evaluation (i.e. actually playing)               */
/* -------------------------------------------------------------------------- */

// TOKEEP: for debugging purposes
void Game::search_random(U64& bestMove) {
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
// int Game::search_best_alpha_beta(U64& bestMove, int depth, int alpha, int beta) {
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

int Game::search_best_minimax(U64& bestMove, int depth) {
    if (depth == 0) return evaluate();

    static long nodes_searched = 0;
    if (depth == MAX_ALPHA_BETA_DEPTH) {
        nodes_searched = 0;  // Reset counter at root
    }
    nodes_searched++;

    int bestValue = (board.turn == W) ? -99999 : 99999;
    BitMoveVec moves = board.get_all_legal_moves();
    
    for (const auto& move : moves) {
        BoardState savedState(board);
        if (board.move(move) == -1) continue;
        
        int score = search_best_minimax(bestMove, depth - 1);
        savedState.reapply(board);

        if (board.turn == W) {  // Maximizing player
            if (score > bestValue) {
                bestValue = score;
                if (depth == MAX_ALPHA_BETA_DEPTH) {
                    bestMove = move.get_bit_repr();
                }
            }
        } else {  // Minimizing player 
            if (score < bestValue) {
                bestValue = score;
                if (depth == MAX_ALPHA_BETA_DEPTH) {
                    bestMove = move.get_bit_repr();
                }
            }
        }
    }

    if (depth == MAX_ALPHA_BETA_DEPTH) {
        std::cout << "[MINIMAX]Total nodes searched: " << nodes_searched << std::endl;
        return 0;
    }

    return bestValue;
}

int Game::search_best_alpha_beta(U64& bestMove, int depth, int alpha, int beta) {
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
        score = search_best_alpha_beta(bestMove, depth - 1, alpha, beta);
        savedState.reapply(board);

        if (board.turn == W) {  // maximizing player
            if (score > bestValue) {
                bestValue = score;
                if (depth == MAX_ALPHA_BETA_DEPTH) {
                    bestMove = move.get_bit_repr();
                }
                if (score > alpha) {
                    alpha = score;
                }
            }
        } else {  // minimizing player
            if (score < bestValue) {
                bestValue = score;
                if (depth == MAX_ALPHA_BETA_DEPTH) {
                    bestMove = move.get_bit_repr();
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


int Game::evaluate() {
    int evaluation = 0;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            evaluation += evaluate_piece(board.pieceOnSquare[i * 8 + j], i, j);

    // TODO: crucial: use justCheckCheck on board.move()
    // if black checks white
    if (board.is_attacked(board.find_king(W), B)) {
        evaluation -= 50;
        BitMoveVec moves = board.get_all_legal_moves();
        if (moves.size() == 0) return -99999;

    } else if (board.is_attacked(board.find_king(B), W)) {  // if white checks black
        evaluation += 50;
        BitMoveVec moves = board.get_all_legal_moves();
        if (moves.size() == 0) return 99999;
    }
    return evaluation;
}

int Game::evaluate_piece(int piece, int rank, int file) {
    	static const int pawn_table[8][8] = {
	    {  0,  0,  0,  0,  0,  0,  0,  0 },
	    { 50, 50, 50, 50, 50, 50, 50, 50 },
	    { 10, 10, 20, 30, 30, 20, 10, 10 },
	    {  5,  5, 10, 25, 25, 10,  5,  5 },
	    {  0,  0,  0, 20, 20,  0,  0,  0 },
	    {  5, -5, -10,  0,  0, -10, -5,  5 },
	    {  5, 10, 10, -20, -20, 10, 10,  5 },
	    {  0,  0,  0,  0,  0,  0,  0,  0 }
	};
	
	static const int knight_table[8][8] = {
	    { -50, -40, -30, -30, -30, -30, -40, -50 },
	    { -40, -20,   0,   0,   0,   0, -20, -40 },
	    { -30,   0,  10,  15,  15,  10,   0, -30 },
	    { -30,   5,  15,  20,  20,  15,   5, -30 },
	    { -30,   0,  15,  20,  20,  15,   0, -30 },
	    { -30,   5,  10,  15,  15,  10,   5, -30 },
	    { -40, -20,   0,   5,   5,   0, -20, -40 },
	    { -50, -40, -30, -30, -30, -30, -40, -50 }
	};

	static const int bishop_table[8][8] = {
	    { -20, -10, -10, -10, -10, -10, -10, -20 },
	    { -10,   0,   0,   0,   0,   0,   0, -10 },
	    { -10,   0,   5,  10,  10,   5,   0, -10 },
	    { -10,   5,   5,  10,  10,   5,   5, -10 },
	    { -10,   0,  10,  10,  10,  10,   0, -10 },
	    { -10,  10,  10,  10,  10,  10,  10, -10 },
	    { -10,   5,   0,   0,   0,   0,   5, -10 },
	    { -20, -10, -10, -10, -10, -10, -10, -20 }
	};

	static const int rook_table[8][8] = {
	    {  0,  0,  0,  0,  0,  0,  0,  0 },
	    {  5, 10, 10, 10, 10, 10, 10,  5 },
	    { -5,  0,  0,  0,  0,  0,  0, -5 },
	    { -5,  0,  0,  0,  0,  0,  0, -5 },
	    { -5,  0,  0,  0,  0,  0,  0, -5 },
	    { -5,  0,  0,  0,  0,  0,  0, -5 },
	    {  5, 10, 10, 10, 10, 10, 10,  5 },
	    {  0,  0,  0,  5,  5,  0,  0,  0 }
	};

	static const int queen_table[8][8] = {
	    { -20, -10, -10,  -5,  -5, -10, -10, -20 },
	    { -10,   0,   0,   0,   0,   0,   0, -10 },
	    { -10,   0,   5,   5,   5,   5,   0, -10 },
	    {  -5,   0,   5,   5,   5,   5,   0,  -5 },
	    {   0,   0,   5,   5,   5,   5,   0,  -5 },
	    { -10,   5,   5,   5,   5,   5,   0, -10 },
	    { -10,   0,   5,   0,   0,   0,   0, -10 },
	    { -20, -10, -10,  -5,  -5, -10, -10, -20 }
	};

	static const int king_table[8][8] = {
	    { -30, -40, -40, -50, -50, -40, -40, -30 },
        { -30, -40, -40, -50, -50, -40, -40, -30 },
        { -30, -40, -40, -50, -50, -40, -40, -30 },
        { -30, -40, -40, -50, -50, -40, -40, -30 },
        { -20, -30, -30, -40, -40, -30, -30, -20 },
        { -10, -20, -20, -20, -20, -20, -20, -10 },
        {  20,  20,   0,   0,   0,   0,  20,  20 },
        {  20,  30,  10,   0,   0,  10,  30,  20 }
	};

    if (piece == 12) return 0;
    int piecetype = piece % 6;
    int color = (piece > 5) ? 0 : 1;

    static const int material_values[] = {100, 320, 330, 500, 900, 10000};
    int base_value = material_values[piecetype];

    int positional_bonus = 0;
    switch (piecetype) {
        case PAWN: positional_bonus = color ? pawn_table[rank][file] : pawn_table[7 - rank][file]; break;
        case KNIGHT: positional_bonus = color ? knight_table[rank][file] : knight_table[7 - rank][file]; break;
        case BISHOP: positional_bonus = color ? bishop_table[rank][file] : bishop_table[7 - rank][file]; break;
        case ROOK: positional_bonus = color ? rook_table[rank][file] : rook_table[7 - rank][file]; break;
        case QUEEN: positional_bonus = color ? queen_table[rank][file] : queen_table[7 - rank][file]; break;
        case KING: positional_bonus = color ? king_table[rank][file] : king_table[7 - rank][file]; break;
    }
    int value = base_value + positional_bonus;
    return (color ? value : -value);
}

// get opening move
std::string Game::playOpeningMove(int argc, char** argv) {
    std::string bestMoveStr = openings->getMove(board);
    if (!bestMoveStr.empty()) {
        // transform a move in alg notation (ex : e2e4) to a bitmove
        Parser parser(board, argc, argv);
        BitMove bestMove = parser.parse_algebraic_move(
            coord_to_sq(bestMoveStr.substr(0, 2)), coord_to_sq(bestMoveStr.substr(2, 2)), ' ');
        board.move(bestMove);  // playing opening move
        return bestMove.get_algebraic_notation();
    }
    return "";
    // else no opening move, proceed with normal search
}
