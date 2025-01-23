#pragma once

#include "../Board.h"
#include "../BitMove.h"

namespace Knight {
    U64 get_attack_mask(int square);
    void add_legal_moves(Board &board, MoveList &moves, bool onlyCaptures = false);
};
