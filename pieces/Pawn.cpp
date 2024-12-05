#include "Pawn.h"
#include <iostream>

Pawn::Pawn(int color, Pos pos) {
	this->type = 0;
	this->color = color;
	this->pos = pos;
}

bool Pawn::has_moved() {
	if (color == 1)
		return (pos.y == 1);
	else
		return (pos.y == 6);
}

std::vector<std::string> Pawn::legal_moves(Piece* board[8][8]) {
	std::vector<std::string> store;
	int d = color ? 1 : -1;
	Pos newpos {pos.x+d,pos.y};

	if (board[newpos.x][newpos.y] == nullptr) // move one square
		store.push_back(pos.to_str()+newpos.to_str());
        
	newpos.x += d;
	if (!has_moved() && board[newpos.x][newpos.y] == nullptr) // move two squares
		store.push_back(pos.to_str()+newpos.to_str());
	// captures
	newpos.x -= d;
	newpos.y++;	
	if (newpos.in_bound() && board[newpos.x][newpos.y] != nullptr)
		if (board[newpos.x][newpos.y]->color != color)
			store.push_back(pos.to_str()+newpos.to_str());

	newpos.y -= 2;
	if (newpos.in_bound() && board[newpos.x][newpos.y] != nullptr)
		if (board[newpos.x][newpos.y]->color != color)
			store.push_back(pos.to_str()+newpos.to_str());

	return store;
}
