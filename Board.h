#pragma once
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"
#include "pieces/Queen.h"

struct Board {
  Piece* board[8][8];
  //read the origin's position of a move
  Pos str_from(std::string);
  //read the destination's position of a move
  Pos str_to(std::string);

  Board();
 
 //returns a list of all legal moves 
  std::vector<std::string> all_legal_moves(int color);
  //move a pawn and return what the captured pawn(if there is)
  Piece* move(Pos from, Pos to);
  void undo_move(Pos from, Pos to, Piece* captured); 
  //check if there is a check situation
  bool in_check(int color);
  void display();
  void init();
};
