#include "Bishop.h"
#include <cmath>

Bishop::Bishop(int color, Pos pos) {
	this->type = 2;
	this->color = color;
	this->pos = pos;
}

std::vector<std::string> Bishop::legal_moves(Piece* board[8][8]) {
	std::vector<std::string> store;
	Pos newpos {pos.x,pos.y};
	//1st diagonal
	for (int d = 1; d < 8; d++) {
		newpos.x = pos.x+d;
		newpos.y = pos.y+d;
		//check if it is on the board
		if (!newpos.in_bound()) break;
		
		//check if a pawn of our color blocks us
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		//add to legal moves
		store.push_back(pos.to_str() + newpos.to_str());
		//if a pawn of the opposite color is here, we can't move further
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;	
	}
	//2nd diagonal
	for (int d = 1; d < 8; d++) {
		newpos.x = pos.x+d;
		newpos.y = pos.y-d;
		if (!newpos.in_bound()) break;
		
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;	
	}
	//3rd diagonal
	for (int d = 1; d < 8; d++) {
		newpos.x = pos.x-d;
		newpos.y = pos.y+d;
		if (!newpos.in_bound()) break;
		
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;	
	}
	//4th diagonal
	for (int d = 1; d < 8; d++) {
		newpos.x = pos.x-d;
		newpos.y = pos.y-d;
		if (!newpos.in_bound()) break;
		
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;	
	}
	return store;
}
