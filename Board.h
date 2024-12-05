#include "Square.h"
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"
#include "pieces/Queen.h"

struct Board {
  Piece* board[8][8];

  Board();
  
  //bool blocked_move(Pos from, Pos to);
  void display();
  void init();
};
