#include "Game.h"
#include "Parser.h"
#include "pieces/Piece.h"
#include <iostream>

int main(int argc, char **argv) {
  
	
// TODO: all use same clang-format style
	
  Game Chess;
  std::cout << Chess.search(4,1) << std::endl;

  Parser parser{argc, argv};
  parser.parseArgs();
  parser.parseHistory();
  parser.writeNextMove("g1h3");

  return 0;
}
