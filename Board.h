#include "Square.h"
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"
#include "pieces/Queen.h"

struct Board {
  Square board[8][8];

  // bool in_bounds(Pos p);
  Board();

  void display();
  void init();
};
