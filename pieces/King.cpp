#include "King.h"
#include <cmath>

King::King(int color, Pos pos) {
  this->type = 5;
  this->color = color;
  this->pos = pos;
  is_castled = false;
}

std::vector<std::string> King::legal_moves(Piece* board[8][8]) {
	std::vector<std::string> store;
	int dx[8] = {1,1,1,0,0,-1,-1,-1};
	int dy[8] = {1,0,-1,1,-1,1,0,-1};
	for (int i = 0; i < 8; i++) {
		Piece* p = board[pos.x+dx[i]][pos.y+dy[i]];
		Pos newpos = Pos{pos.x+dx[i], pos.y+dy[i]};
		if (!newpos.in_bound()) continue;
		if (p == nullptr || p->get_color() != color) 
			store.push_back(pos.to_str() + newpos.to_str());
	}
	return store;
}
