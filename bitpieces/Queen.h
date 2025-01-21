#pragma once

#include "../Board.h"
#include "../BitMove.h"

namespace Queen {
    U64 get_attack_masks_blocking(int square, U64 blocking_square);
    void add_legal_moves(Board &board, BitMoveVec &moves, bool onlyCaptures = false);
};