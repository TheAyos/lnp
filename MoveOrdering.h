#pragma once
#include <algorithm>
#include <cstring>

#include "Board.h"
#include "BitMove.h"
#include "PV.h"

namespace MoveOrdering {
void Killer(MoveList &moves, BitMove killer_move) {
    for (size_t i = moves.attacks()+1; i < moves.size(); i++) {
	if (moves[i] == killer_move) {
	    std::memmove(&moves[moves.attacks()+1], &moves[moves.attacks()], (i-moves.attacks())*sizeof(BitMove));
	    moves[moves.attacks()] = killer_move;
	    break;
	}
    }
}

void MVV_LVA(Board &board, MoveList &moves) {
    std::sort(moves.begin(), &moves[moves.attacks()], [&board](BitMove left, BitMove right) {
	int left_capture = board.get_piece_on_square(left.get_to());
	int right_capture = board.get_piece_on_square(right.get_to());
	if (left_capture > right_capture) return true;
	int left_piece = board.get_piece_on_square(left.get_from());
	int right_piece = board.get_piece_on_square(right.get_from());
	if (left_capture == right_capture && left_piece < right_piece) return true;
	return false;
    });
}

void PV_P(Board &board, PV &pv, MoveList &moves) {
    size_t pv_index = board.ply - pv.ply();
    if (pv_index > pv.size() || pv.size() == 0) return;
    BitMove pv_move = pv[pv_index];
    for (size_t i = 1; i < moves.size(); i++) {
	if (moves[i] == pv_move) {
	    std::memmove(&moves[1], &moves[0], i* sizeof(BitMove));
	    moves[0] = pv_move;
	    break;
	}
    }
}

void TTPriority(BitMove move, MoveList &moves) {
    for (size_t i = 1; i < moves.size(); i++) {
	if (moves[i] == move) {
	    std::memmove(&moves[1], &moves[0], i*sizeof(BitMove));
	    moves[0] = move;
	    break;
	}
    }	
}

void All(Board &board, BitMove ttmove, PV &pv, MoveList &moves) {
    MVV_LVA(board, moves);
    TTPriority(ttmove, moves);
    PV_P(board, pv, moves);
}
} // namespace MoveOrdering
