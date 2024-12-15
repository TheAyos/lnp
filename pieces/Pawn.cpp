#include "Pawn.h"
#include <iostream>

Pawn::Pawn(int color, Pos pos) {
	this->type = 0;
	this->color = color;
	this->pos = pos;
	this->just_moved = false;
	this->has_moved = false;
}

bool Pawn::has_moved1() {
	if (color == 1) return (pos.x != 1);
	else return (pos.x != 6);
}

std::vector<std::string> Pawn::legal_moves(Piece* board[8][8]) {
	std::vector<std::string> store;
	int d = color ? 1 : -1;
	Pos newpos {pos.x+d,pos.y};
	
	std::string promotion_str = "kbrq";

	//move +1 to an empty square
	if (newpos.in_bound() && board[newpos.x][newpos.y] == nullptr) {
		std::string move_str = pos.to_str()+newpos.to_str();
		if (newpos.x == 0 || newpos.x == 7) // reach the end
			for (int i = 0; i < 4; i++) store.push_back(move_str+promotion_str[i]);
		else store.push_back(move_str);
	}
    
	//move +2 to an empty square if the pawn has not moved
	newpos.x += d;
	if (!has_moved1() && board[newpos.x][newpos.y] == nullptr && board[newpos.x-d][newpos.y] == nullptr) 
		store.push_back(pos.to_str()+newpos.to_str());
	
	//check if we can capture an opponent
	//right
	newpos.x -= d;
	newpos.y++;	
	if (newpos.in_bound() && board[newpos.x][newpos.y] != nullptr)
		if (board[newpos.x][newpos.y]->color != color) {
			std::string move_str = pos.to_str()+newpos.to_str();
			if (newpos.x == 0 || newpos.x == 7) // reach the end
				for (int i = 0; i < 4; i++) store.push_back(move_str+promotion_str[i]);
			else store.push_back(move_str);	
		}
	
	//left
	newpos.y -= 2;
	if (newpos.in_bound() && board[newpos.x][newpos.y] != nullptr)
		if (board[newpos.x][newpos.y]->color != color) {
			std::string move_str = pos.to_str()+newpos.to_str();
			if (newpos.x == 0 || newpos.x == 7) // reach the end
				for (int i = 0; i < 4; i++) store.push_back(move_str+promotion_str[i]);
			else store.push_back(move_str);	
		}
	
	return store;
}
