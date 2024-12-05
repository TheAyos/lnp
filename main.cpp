#include <iostream>
#include <string>
#include "Piece.h"
#include "Board.h"
#include "Square.h"

int main(int argc, char **argv) {
	Board b;
	b.display();
	
	Piece* wk = b.board[0][4].piece;
	Pos p {0,6};
	std::cout << wk->potential_move(p) << std::endl;
  	return 0;
}
