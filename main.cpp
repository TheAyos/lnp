#include "Board.h"
#include "Parser.h"
#include "Square.h"
#include "pieces/Piece.h"
#include <iostream>

int main(int argc, char **argv) {
  Board b;
  b.display();

  Piece *wk = b.board[0][4].piece;
  Pos p{0, 6};
  std::cout << wk->potential_move(p) << std::endl;

  // TODO: all use same clang-format style

  Parser parser{argc, argv};
  parser.parseArgs();
  parser.parseHistory();
  parser.writeNextMove("g1h3");

  return 0;
}
