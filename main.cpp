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

  return 0;
}

/*
How to open a file for reading :
std::ifstream inputFile("input.txt");

How to open a file for writing :
std::ofstream outputFile("output.txt");

Read a line from the file :
std::getline(inputFile, line)
--> while (std::getline(inputFile, line))
it reads each line from the file and stores it in the line string, it continues
until the end of the file is reached
*/
