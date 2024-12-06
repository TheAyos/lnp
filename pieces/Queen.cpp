#include "Queen.h"
#include <cmath>

Queen::Queen(int color, Pos pos) {
    this->type = 4;
    this->color = color;
    this->pos = pos;
}

std::vector<std::string> Queen::legal_moves(Piece* board[8][8]) {
	std::vector<std::string> store;
	Pos newpos {pos.x,pos.y};
	for (int dx = 1; dx < 8; dx++) {
		newpos.x = pos.x+dx;
		if (!newpos.in_bound()) break;
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;
	}
	for (int dx = 1; dx < 8; dx++) {
		newpos.x = pos.x-dx;
		if (!newpos.in_bound()) break;
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;
	}
	newpos.x = pos.x;
	for (int dy = 1; dy < 8; dy++) {
		newpos.y = pos.y+dy;
		if (!newpos.in_bound()) break;
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;
	}
	for (int dy = 1; dy < 8; dy++) {
		newpos.y = pos.y-dy;
		if (!newpos.in_bound()) break;
		if (board[newpos.x][newpos.y] != nullptr && board[newpos.x][newpos.y]->color == color) break;
		
		store.push_back(pos.to_str() + newpos.to_str());
		if (board[newpos.x][newpos.y] == nullptr) continue;
		break;
	}
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
