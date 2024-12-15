#pragma once
#include <iostream>
#include <vector>
#include <string>

struct Pos {
	int x;
	int y;

	std::string to_str();
	bool in_bound();
	bool operator==(const Pos& other) const;
};

struct Board;

struct Piece {
	Pos pos;
	int color, type;
	bool has_moved, just_moved;
	/*
	 * color: 0 black, 1 white
	 * type: 0 pawn, 1 knight, 2 bishop, 3 rook, 4 queen, 5 king
	 * pos refers to position
	 */

	Piece() : pos {0,0}, color{0}, type{0} {};

	void set_pos(Pos pos);
	Pos get_pos();
	int get_color();
	int get_type();
	virtual std::vector<std::string> legal_moves(Piece* board[8][8]) = 0;
};