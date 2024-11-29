#include "Game.h"

Game::Game() {
	game_over = false;
	turn = 1;

	King* w_king = new King(1);
	King* b_king = new King(0);
	w_Pieces.push_back(w_king);
	b_Pieces.push_back(b_king);
	
	for ( int row = 0; row < 8; row++ )
		for ( int col = 0; col < 8; col++ ) {
			Board[row][col].row = row;
			Board[row][col].col = col;
		}

	Board[0][4].color_id = 1;
	Board[0][4].piece_id = 6;
	Board[7][4].color_id = 0;
	Board[7][4].piece_id = 6;
}

std::vector<Square> Game::get_moves(Pieces* p) {
	std::vector<Square> moves;
	// std::vector<Pieces*> op_Pieces = p->my_color ? b_Pieces : w_Pieces;
	
	for ( auto & m : p->potential_moves(Board) ) {
		int row = m.row;
		int col = m.col;
		int drow[] = {1,1,1,0,0,-1,-1,-1};
		int dcol[] = {1,0,-1,1,-1,1,0,-1};
		bool safe = true;
		for ( int i = 0; i < 8; i++ ) {
			int nrow = row+drow[i];
			int ncol = col+dcol[i];
			// std::cout << nrow << " " << ncol << std::endl;
			if (nrow>=0 && nrow < 8 && ncol >=0 && ncol<8)
				if (Board[nrow][ncol].piece_id == 6 && Board[nrow][ncol].color_id != p->my_color)
					safe = false; // near another king
		}
		if (safe) moves.push_back(m);
	}
	return moves;
}

void Game::move(Pieces* p, int row, int col) {
	std::vector<Square> moves = get_moves(p);
	bool legal = false;
	for (auto & sq : moves) {
		if (sq.row == row && sq.col == col)
			legal = true;
	}

	if (legal) {
		Board[row][col].piece_id = Board[p->my_row][p->my_col].piece_id;  
		Board[row][col].color_id = Board[p->my_row][p->my_col].color_id;
		Board[p->my_row][p->my_col].piece_id = -1;
		Board[p->my_row][p->my_col].color_id = -1;
		p->my_row = row;
		p->my_col = col;
	}
	else {
		std::cout << "Illegal move" << std::endl;
		exit(0);
	}
}

void Game::display() {
	std::string line = "";
	for (int i = 0; i < 8; i++) line += " ----";
	line += "\n";

	std::string out = line;
	for (int row = 7; row >= 0; row--) {
		for (int col = 0; col < 8; col++) {
			out += "| ";
			Square cur_sq = Board[row][col];
			if (cur_sq.color_id == -1 ) // Empty square:
				out += "   ";
			else {
				if (cur_sq.color_id == 0) // Black piece
					out += "B";
				else // White piece
					out += "W";
				if (cur_sq.piece_id == 6) // King
					out += "K ";
			}
		}
		out += "|\n"+line;
	}
	std::cout << out << std::endl;
}
