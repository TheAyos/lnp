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

	for (int d = 1; d < 8; d++) {
		newpos.x = pos.x+d;
		newpos.y = pos.y+d;
		if (!newpos.in_bound()) break;
		
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;	
	}
	for (int d = 1; d < 8; d++) {
		newpos.x = pos.x+d;
		newpos.y = pos.y-d;
		if (!newpos.in_bound()) break;
		
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;	
	}
	for (int d = 1; d < 8; d++) {
		newpos.x = pos.x-d;
		newpos.y = pos.y+d;
		if (!newpos.in_bound()) break;
		
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;	
	}
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
