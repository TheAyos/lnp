#include "Pawn.h"
#include <iostream>

Pawn::Pawn(int color, Pos pos) {
	this->type = 0;
	this->color = color;
	this->pos = pos;
}

bool Pawn::has_moved() {
	if (color == 1)
		return (pos.x != 1);
	else
		return (pos.x != 6);
}

bool Pawn::check_promotion(Pos* pos, int color) {
	if ((pos->y == 7 && color == 1) ||(pos->y == 0 && color == 0)) {
		return true;
	}
}

void Pawn::promotion(Piece* board[8][8]) {
	board[pos.x][pos.y] = Queen(color,pos);
}

std::vector<std::string> Pawn::legal_moves(Piece* board[8][8]) {
	std::vector<std::string> store;
	int d = color ? 1 : -1;
	Pos newpos {pos.x+d,pos.y};

	//move +1 to an empty square
	if (newpos.in_bound() && board[newpos.x][newpos.y] == nullptr){
		store.push_back(pos.to_str()+newpos.to_str());
		if (check_promotion(pos,color)) {
			promotion(board);
		}
	}

    
	//move +2 to an empty square if the pawn has not moved
	newpos.x += d;
	if (!has_moved() && board[newpos.x][newpos.y] == nullptr && board[newpos.x-d][newpos.y] == nullptr) 
		store.push_back(pos.to_str()+newpos.to_str());
	
	//check if we can capture an opponent
	//right
	newpos.x -= d;
	newpos.y++;	
	if (newpos.in_bound() && board[newpos.x][newpos.y] != nullptr)
		if (board[newpos.x][newpos.y]->color != color){
			store.push_back(pos.to_str()+newpos.to_str());
			if (check_promotion(pos,color)) {
				promotion(board);
			}
		}


	//left
	newpos.y -= 2;
	if (newpos.in_bound() && board[newpos.x][newpos.y] != nullptr)
		if (board[newpos.x][newpos.y]->color != color){
			store.push_back(pos.to_str()+newpos.to_str());
			if (check_promotion(pos,color)) {
				promotion(board);
			}
		}	

	return store;
}
