#pragma once
#include "BitBoard.h"

class Piece {
   public:
    virtual U64 get_attack_mask() = 0;
};