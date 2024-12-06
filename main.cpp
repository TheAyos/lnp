#include "Board.h"
#include "Parser.h"
#include "Square.h"
#include "pieces/Piece.h"
#include <iostream>

int main(int argc, char **argv) {
  Board b;
  b.display();

  b.board[4][5] = new King{1, Pos{4,5}};
  Piece* wn = b.board[4][5];
  // b.board[2][3] = new Pawn{1, Pos{2,3}};
  // b.display();
  
  std::vector<std::string> moves;
  moves = wn->legal_moves(b.board);
  for ( auto & element : moves )
	  std::cout << element << std::endl;
  std::cout << wn->get_color() << std::endl;

  // TODO: all use same clang-format style

  Parser parser{argc, argv};
  parser.parseArgs();
  parser.parseHistory();
  parser.writeNextMove("g1h3");

  return 0;
}
