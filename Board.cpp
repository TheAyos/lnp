#include "Board.h"
#include <string>
#include <iostream>

Board::Board() { init(); }
/*
// checks if a piece is blocking the move 
// DOES NOT CHECK THE DESTINATION!!, that is checked in the individual pieces
bool Board::blocked_move(Pos from, Pos to) { // check if a piece is blocking the move 
	if (abs(from.x-to.x) <= 1 && abs(from.y-to.y) <= 1) return false; // one square move
	
	if (board[from.x][from.y].get_piece()->get_type() == 1) return false; // knights can jump
	
	if (from.x == to.x) { // horizontal move
		if (from.y < to.y)
			for (int j = from.y+1; j < to.y; j++) {
				Piece* p = board[to.x][j].get_piece();
				if (p != nullptr) return true;
			}
		else
			for (int j = to.y+1; j < from.y; j++){
				Piece* p = board[to.x][j].get_piece();
				if (p != nullptr) return true;
			}
	}
	else if (from.y == to.y) { // horizontal move
		if (from.x < to.x)
			for (int i = from.x+1; i < to.x; i++) {
				Piece* p = board[i][to.y].get_piece();
				if (p != nullptr) return true;
			}
		else
			for (int i = to.x+1; i < from.x; i++){
				Piece* p = board[i][to.y].get_piece();
				if (p != nullptr) return true;
			}
	}
	else { // diagonal move
	        int i, j;
		if (from.y < to.y && from.x < to.x) { // forward right
			i = from.x+1; j = from.y+1;
			while (i < to.x) {
				Piece* p = board[i][j].get_piece();
				if (p != nullptr) return true;
				i++; j++;
			}
		}
	}
		
}
*/

/*
 * Below are done
 * 
 */
void Board::init() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			Pos p {i, j};
			board[i][j] = nullptr;
		}

	// init kings
	Pos wkp {0,4};
	Pos bkp {7,4};
	board[0][4] = new King{1,wkp};
	board[7][4] = new King{0,bkp};

	// init pawns
	for (int j = 0; j < 8; j++) {
		Pos wpp {1,j};
		Pos bpp {6,j};
		board[1][j] = new Pawn{1,wpp};
		board[6][j] = new Pawn{0,bpp};
	}

	// init knights
	Pos wnp1 {0,1};
	Pos wnp2 {0,6};
	Pos bnp1 {7,1};
	Pos bnp2 {7,6};
	board[0][1] = new Knight{1,wnp1};
	board[0][6] = new Knight{1,wnp2};
	board[7][1] = new Knight{0,bnp1};
	board[7][6] = new Knight{0,bnp2};

	// init bishops
	Pos wbp1 {0,2};
	Pos wbp2 {0,5};
	Pos bbp1 {7,2};
	Pos bbp2 {7,5};
	board[0][2] = new Bishop{1,wbp1};
	board[0][5] = new Bishop{1,wbp2};
	board[7][2] = new Bishop{0,bbp1};
	board[7][5] = new Bishop{0,bbp2};
	
	// init rooks
	Pos wrp1 {0,0};
	Pos wrp2 {0,7};
	Pos brp1 {7,0};
	Pos brp2 {7,7};
	board[0][0] = new Rook{1,wrp1};
	board[0][7] = new Rook{1,wrp2};
	board[7][0] = new Rook{0,brp1};
	board[7][7] = new Rook{0,brp2};

	// init queens
	Pos wqp {0,3};
	Pos bqp {7,3};
	board[0][3] = new Queen{1,wqp};
	board[7][3] = new Queen{0,bqp};
}

void Board::display() {
	std::string line = "";
	for (int i = 0; i < 8; i++) line += " ----";
	line += "\n";

	std::string out = line;
	for (int i = 7; i >= 0; i--){
		for (int j = 0; j < 8; j++) {
			out += "| ";
			Piece* p = board[i][j];
			if (p == nullptr)
				out += "   ";
			else {
				if (p->color == 0)
					out += "b";
				else
					out += "w";
				if (p->type == 0)
					out += "P ";
				if (p->type == 1)
					out += "N ";
				if (p->type == 2)
					out += "B ";
				if (p->type == 3)
					out += "R ";
				if (p->type == 4)
					out += "Q ";
				if (p->type == 5)
					out += "K ";
			}
		}
		out += "|\n"+line;
	}
	std::cout << out << std::endl;
}
