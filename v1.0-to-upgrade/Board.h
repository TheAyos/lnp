#pragma once
#include "pieces/King.h"
#include "pieces/bitpieces/Pawn.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"
#include "pieces/Queen.h"

struct OldBoard {
  Piece* board[8][8];
  //read the origin's position of a move
  Pos str_from(std::string);
  //read the destination's position of a move
  Pos str_to(std::string); 
  Pos find_king(int color);

  OldBoard();
 
 //returns a list of all legal moves 
  std::vector<std::string> all_legal_moves(int color);

  // moving and undoing move mechanisms
  void move(std::string move_str, int* board_status);
  void undo_move(std::string move_str, int* board_status);

  /*
  void undo_move(Pos from, Pos to, Piece* captured);
  void castle(Pos from, Pos to);
  void undo_castle(Pos from, Pos to);
  Piece* enp(Pos from, Pos to);
  void undo_enp(Pos from, Pos to, Piece* captured);
  */
  //check if there is a check situation
  bool in_check(int color);
  // mic
  void display();
  void init();
};
