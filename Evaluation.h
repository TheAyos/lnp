#pragma once
#include "Board.h"
#include "BitOps.h"
#include "Evaluator.h"

namespace Evaluation {
namespace {
std::pair<int, int> determine_phase(Board &board) {
    int game_phase = 0;
    
    for (int piece_type = 0; piece_type < 12; piece_type++ ) {
        U64 bb = board.bitboards[piece_type];
	int count = BitOps::pop_count_bits(bb);
	game_phase += count * Evaluator::PHASE[piece_type%6];
    }

    int middle = std::min(24, game_phase);
    int end = 24 - middle;
    return {middle, end};
}

template <int color>
std::pair<int, int> evaluate_piece_sq(Board &board) {
    int middle = 0;
    int end = 0;

    for (int i = 0; i < 64; i++) {
	int piece_type = board.pieceOnSquare[i];
	if (piece_type == 12) continue;
	if (color == 0 && piece_type < 6) continue;
	if (color == 1 && piece_type > 5) continue;
	
	// std::cout << piece_type << std::endl;
        middle += Evaluator::MG[color][piece_type%6][i];
	end += Evaluator::EG[color][piece_type%6][i];
    }

    return {middle, end}; 
}

std::pair <int, int> evaluate_piece_sq(Board &board) {
    auto white = evaluate_piece_sq<1>(board);
    // std::cout << white.first << std::endl;
    auto black = evaluate_piece_sq<0>(board);
    // std::cout << black.first << std::endl;
    return {white.first-black.first, white.second-black.second};
}

int eval(Board &board) {
    const std::pair<int, int> game_phase = determine_phase(board);

    int valuation = 0;
    const std::pair<int, int> piece_sq = evaluate_piece_sq(board);
    valuation += game_phase.first * piece_sq.first;
    valuation += game_phase.second * piece_sq.second;
    valuation /= 24;

    return valuation;
}

} // namespace
} // namespace Evaluation
