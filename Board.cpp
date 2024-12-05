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

	// init pawns
	for (int j = 0; j < 8; j++) {
		Pos wpp {1,j};
		Pos bpp {6,j};
		board[1][j].set_piece(new Pawn{1,wpp});
		board[6][j].set_piece(new Pawn{0,bpp});
	}

	// init knights
	Pos wnp1 {0,1};
	Pos wnp2 {0,6};
	Pos bnp1 {7,1};
	Pos bnp2 {7,6};
	board[0][1].set_piece(new Knight{1,wnp1});
	board[0][6].set_piece(new Knight{1,wnp2});
	board[7][1].set_piece(new Knight{0,bnp1});
	board[7][6].set_piece(new Knight{0,bnp2});

	// init bishops
	Pos wbp1 {0,2};
	Pos wbp2 {0,5};
	Pos bbp1 {7,2};
	Pos bbp2 {7,5};
	board[0][2].set_piece(new Bishop{1,wbp1});
	board[0][5].set_piece(new Bishop{1,wbp2});
	board[7][2].set_piece(new Bishop{0,bbp1});
	board[7][5].set_piece(new Bishop{0,bbp2});
	
	// init rooks
	Pos wrp1 {0,0};
	Pos wrp2 {0,7};
	Pos brp1 {7,0};
	Pos brp2 {7,7};
	board[0][0].set_piece(new Rook{1,wrp1});
	board[0][7].set_piece(new Rook{1,wrp2});
	board[7][0].set_piece(new Rook{0,brp1});
	board[7][7].set_piece(new Rook{0,brp2});

	// init queens
	Pos wqp {0,3};
	Pos bqp {7,3};
	board[0][3].set_piece(new Queen{1,wqp});
	board[7][3].set_piece(new Queen{0,bqp});
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
				if (sq.piece->type == 0)
					out += "P ";
				if (sq.piece->type == 1)
					out += "N ";
				if (sq.piece->type == 2)
					out += "B ";
				if (sq.piece->type == 3)
					out += "R ";
				if (sq.piece->type == 4)
					out += "Q ";
				if (sq.piece->type == 5)
					out += "K ";
			}
		}
		out += "|\n"+line;
	}
	std::cout << out << std::endl;
}
