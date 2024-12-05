#include "Board.h"
#include <string>
#include <iostream>

Board::Board() { init(); }

void Board::init() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			Pos p {i, j};
			board[i][j].set_piece(nullptr);
		}

	// init kings
	Pos wkp {0,4};
	Pos bkp {7,4};
	board[0][4].set_piece(new King{1,wkp});
	board[7][4].set_piece(new King{0,bkp});
}

void Board::display() {
	std::string line = "";
	for (int i = 0; i < 8; i++) line += " ----";
	line += "\n";

	std::string out = line;
	for (int i = 7; i >= 0; i--){
		for (int j = 0; j < 8; j++) {
			out += "| ";
			Square sq = board[i][j];
			if (sq.piece == nullptr)
				out += "   ";
			else {
				if (sq.piece->color == 0)
					out += "b";
				else
					out += "w";
				if (sq.piece->type == 5)
					out += "K ";
			}
		}
		out += "|\n"+line;
	}
	std::cout << out << std::endl;
}
