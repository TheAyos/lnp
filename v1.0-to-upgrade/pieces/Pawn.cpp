// #include "bitpieces/Pawn.h"
// #include <iostream>

// Pawn::Pawn(int color, Pos pos) {
// 	this->type = 0;
// 	this->color = color;
// 	this->pos = pos;
// 	this->just_moved = false;
// 	this->has_moved = false;
// }

// bool Pawn::has_moved1() {
// 	if (color == 1) return (pos.x != 1);
// 	else return (pos.x != 6);
// }

// std::vector<std::string> Pawn::legal_moves(Piece* board[8][8]) {
// 	std::vector<std::string> store;
// 	int d = color ? 1 : -1;
// 	Pos newpos {pos.x+d,pos.y};
	
// 	std::string promotion_str = "kbrq";

// 	//move +1 to an empty square
// 	if (newpos.in_bound() && board[newpos.x][newpos.y] == nullptr) {
// 		std::string move_str = pos.to_str()+newpos.to_str();
// 		if (newpos.x == 0 || newpos.x == 7) // reach the end
// 			for (int i = 0; i < 4; i++) store.push_back(move_str+promotion_str[i]);
// 		else store.push_back(move_str);
// 	}
    
// 	//move +2 to an empty square if the pawn has not moved
// 	newpos.x += d;
// 	if (!has_moved1() && board[newpos.x][newpos.y] == nullptr && board[newpos.x-d][newpos.y] == nullptr) 
// 		store.push_back(pos.to_str()+newpos.to_str());
// 	// captures
// 	newpos.x -= d;
// 	newpos.y++;	
// 	if (newpos.in_bound() && board[newpos.x][newpos.y] != nullptr)
// 		if (board[newpos.x][newpos.y]->color != color) {
// 			std::string move_str = pos.to_str()+newpos.to_str();
// 			if (newpos.x == 0 || newpos.x == 7) // reach the end
// 				for (int i = 0; i < 4; i++) store.push_back(move_str+promotion_str[i]);
// 			else store.push_back(move_str);	
// 		}
	
// 	//left
// 	newpos.y -= 2;
// 	if (newpos.in_bound() && board[newpos.x][newpos.y] != nullptr)
// 		if (board[newpos.x][newpos.y]->color != color) {
// 			std::string move_str = pos.to_str()+newpos.to_str();
// 			if (newpos.x == 0 || newpos.x == 7) // reach the end
// 				for (int i = 0; i < 4; i++) store.push_back(move_str+promotion_str[i]);
// 			else store.push_back(move_str);	
// 		}
	
// 		// if (board[newpos.x][newpos.y]->color != color)
// 		// if (board[newpos.x][newpos.y]->color != color)
// 		// 	store.push_back(pos.to_str()+newpos.to_str());

// 	// TO REMOVE AFTER FIXING GIT MERGE CONFLICTS
// 	// /* -------------------------------------------------------------------------- */
// 	// /*    en passant attack: board::enPassantSquare/Color, filled in Parser.cpp   */
// 	// /* -------------------------------------------------------------------------- */
// 	// int enpassantmoves = 0;
// 	// if (board[pos.x][pos.y]->color == 1 && pos.x == 4) { // white pawns can only en passant from 5th rank
// 	// 	Pos attack_left = Pos{pos.x+1, pos.y-1};
// 	// 	Pos attack_right = Pos{pos.x+1, pos.y+1};
// 	// 	if (pos.y > 0 && board[pos.x][pos.y-1] != nullptr &&  // Left capture
// 	// 		attack_left == b->enPassantSquare && 
// 	// 		b->enPassantColor == 1 - color) {
// 	// 		moves.push_back(pos.to_str() + attack_left.to_str());
// 	// 		enpassantmoves++;
// 	// 	}
// 	// 	if (pos.y < 7 && board[pos.x][pos.y+1] != nullptr &&  // Right capture
// 	// 		attack_right == b->enPassantSquare && b->enPassantColor == 1 - color) {
// 	// 		moves.push_back(pos.to_str() + attack_right.to_str());
// 	// 		enpassantmoves++;
// 	// 	}
// 	// } else if (pos.x == 3) {  // Black pawn can only en passant from 4rd rank
// 	// 	Pos attack_left = Pos{pos.x-1, pos.y-1};
// 	// 	Pos attack_right = Pos{pos.x-1, pos.y+1};
// 	// 	if (pos.y > 0 && board[pos.x][pos.y-1] != nullptr &&  // Left capture
// 	// 		attack_left == b->enPassantSquare && 
// 	// 		b->enPassantColor == 1 - color) {
// 	// 		moves.push_back(pos.to_str() + attack_left.to_str());
// 	// 		enpassantmoves++;
// 	// 	}
// 	// 	if (pos.y < 7 && board[pos.x][pos.y+1] != nullptr &&  // Right capture
// 	// 		attack_right == b->enPassantSquare && b->enPassantColor == 1 - color) {
// 	// 		moves.push_back(pos.to_str() + attack_right.to_str());
// 	// 		enpassantmoves++;
// 	// 	}
// 	// }
// 	// // TESTING: to use with tests/enpassant_1.txt
// 	// // if (board[pos.x][pos.y]->color == 1)
// 	// // 	std::cout << "Pawn at " << pos.to_str() << " has " << moves.size() << " legal moves." << std::endl;
// 	// // for (const auto& move : moves) 
// 	// // 	std::cout << move << std::endl;
// 	// if (enpassantmoves)
// 	// 	std::cout << "Pawn at " << pos.to_str() << " has " << enpassantmoves << " en passant moves." << std::endl;
// 	// /* -------------------------------------------------------------------------- */
// 	// /*    en passant attack: board::enPassantSquare/Color, filled in Parser.cpp   */
// 	// /* -------------------------------------------------------------------------- */

// 	return store;
// }
