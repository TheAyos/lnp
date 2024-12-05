#include "Square.h"
#include "pieces/King.h"

struct Board {
  Square board[8][8];

  // bool in_bounds(Pos p);
  Board();

  void display();
  void init();
};
