#include "Game.h"
#include "Parser.h"
#include "pieces/Piece.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>

#define TESTING true

int main(int argc, char **argv) {

  srand(time(NULL));
  // TODO: all use same clang-format style

  Game Chess;
  #if TESTING
  {
    // TESTING: perft results
    std::vector<long> reference = {0, 20, 400, 8902, 197281, 4865609, 119060324, 3195901860};
    std::cout << "\nComparison with Perft Results from chessprogramming.org:" << std::endl;
    std::cout << "Depth\tCalculated\tError\tseconds\tKpos/s" << std::endl;
    for (int depth = 1; depth <= 5; depth++) {
      clock_t start = clock();
      int positions = Chess.search(depth, 1);
      clock_t end = clock();
      double time_taken = double(end - start) / CLOCKS_PER_SEC;
      std::cout << depth << "\t" 
                << positions << "\t\t"
                << positions - reference[depth] << "\t"
                << std::setprecision(2) << time_taken << "\t"
                << (positions/10e4 / time_taken)
                << std::endl;
    }
    std::cout << std::endl;
    exit(0);
  }
  #endif

  Parser parser{argc, argv};
  // std::cout << "parsing" << std::endl;
  parser.parseArgs();
  // std::cout << "arguments received" << std::endl;
  int turn = parser.parseHistory(&Chess.my_board);

  // Chess.my_board.display();

  std::cout << "TURN FOR " << (turn ? "black" : "white") << std::endl;
  std::vector<std::string> moves = Chess.my_board.all_legal_moves(turn);
  int rand_int = rand() % moves.size();
  // std::cout << moves[rand_int] << std::endl;
  parser.writeNextMove(moves[rand_int]);

  return 0;
}
