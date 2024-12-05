#include "Knight.h"
#include <cmath>

Knight::Knight(int color, Pos pos) {
	this->type = 1;
	this->color = color;
	this->pos = pos;
}

std::vector<std::string> Knight::legal_moves(Piece* board[8][8]) {
	std::vector<std::string> store;
	int dx[8] = {1,1,2,2,-1,-1,-2,-2};
	int dy[8] = {2,-2,1,-1,2,-2,1,-1};
	for (int i = 0; i < 8; i++) {
		Piece* p = board[pos.x+dx[i]][pos.y+dy[i]];
		Pos newpos = Pos{pos.x+dx[i], pos.y+dy[i]};
		if (!newpos.in_bound()) continue;
		if (p == nullptr || p->get_color() != color) 
			store.push_back(pos.to_str() + newpos.to_str());
	}
	return store;
}
