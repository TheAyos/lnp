#include "Game.h"
#include "Parser.h"
#include "pieces/Piece.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
int main(int argc, char **argv) {

  srand(time(NULL));
  // TODO: all use same clang-format style

  Game Chess;
  // std::cout << Chess.search(4,1) << std::endl;

  Parser parser{argc, argv};
  // std::cout << "parsing" << std::endl;
  parser.parseArgs();
  // std::cout << "arguments received" << std::endl;
  int turn = parser.parseHistory(&Chess.my_board);

  Chess.my_board.display();

  std::cout << "TURN FOR " << turn << std::endl;
  std::vector<std::string> moves = Chess.my_board.all_legal_moves(turn);
  int rand_int = rand() % moves.size();
  // std::cout << moves[rand_int] << std::endl;
  parser.writeNextMove(moves[rand_int]);

  return 0;
}
