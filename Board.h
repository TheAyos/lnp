#pragma once
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"
#include "pieces/Queen.h"

struct Board {
  Piece* board[8][8];
  Pos str_from(std::string);
  Pos str_to(std::string);

  Board();
 
  std::vector<std::string> all_legal_moves(int color);
  Piece* move(Pos from, Pos to);
  void undo_move(Pos from, Pos to, Piece* captured); 
  bool in_check(int color);
  void display();
  void init();
};
