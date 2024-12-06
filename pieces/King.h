#pragma once
#include "Piece.h"

struct King : Piece {
  bool is_castled;

  King(int color, Pos pos);

  std::vector<std::string> legal_moves(Piece* board[8][8]);
};
