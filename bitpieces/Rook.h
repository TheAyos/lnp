#pragma once

#include "../Board.h"
#include "../BitMove.h"

namespace Rook {
    U64 get_attack_masks_blocking_slow(int square, U64 blocking_square);

    U64 get_sliding_mask(int sq);
    void initMagics(Board &board);
    U64 get_attack_masks_blocking_magic(Board& board, int square, U64 blocking_square);

    void add_legal_moves(Board &board, MoveList &moves, bool onlyCaptures = false);
};
