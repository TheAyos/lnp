#include "Board.h"
#include "Util.h"
#include <iostream>
#include <string>

Board::Board() {
  init();
}

Pos Board::str_from(std::string str) {
  return Pos{str[1] - '1', str[0] - 'a'};
}
Pos Board::str_to(std::string str) {
  return Pos{str[3] - '1', str[2] - 'a'};
}

Pos Board::find_king(int color) {
  Pos k_pos{0, 0};
  //take the position of our king
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      if(board[i][j] != nullptr && board[i][j]->type == 5
         && board[i][j]->color == color)
        k_pos = board[i][j]->pos;
  return k_pos;  
}

void Board::move(std::string move_str, int* board_status) { // makes the move, while updating the board_status from before
	// display();
	// std::cout << move_str << std::endl;
	Pos from = str_from(move_str);
	Pos to = str_to(move_str);
	Piece* from_piece = board[from.x][from.y]; // let's assume this is not nullptr
	Piece* to_piece = board[to.x][to.y];
	int color = from_piece->color;
	int promotion = -1;
	if (move_str.length() == 5) {
		if (move_str[4] == 'k') promotion = 1;
		if (move_str[4] == 'b') promotion = 2;
		if (move_str[4] == 'r') promotion = 3;
		if (move_str[4] == 'q') promotion = 4;
	}
	board_status[6] = promotion;
	
	board_status[5] = -1;
	int p_line = (color)?4:3;
	for (int j = 0; j < 8; j++) {
		Piece* pawn = board[p_line][j];
		if (pawn!=nullptr && pawn->type == 0 && pawn->color != color && pawn->just_moved) {
			board_status[5] = j; // the pawn that has just moved
			// my_pawn->just_moved = false; // now we will move something
		}
	} // otherwise no pawn has just moved (board_status[5] stays as -1)	

	if (from_piece->type == 5 && abs(from.y-to.y)==2) { // castle
		from_piece->has_moved = true;
		from_piece->pos.x = to.x;
		from_piece->pos.y = to.y;
		board[to.x][to.y] = from_piece;
		board[from.x][from.y] = nullptr;
		int rook_from_y = (to.y==2)?0:7;
		int rook_to_y = (to.y==2)?3:5;
		board[from.x][rook_from_y]->has_moved = true;
		board[from.x][rook_to_y] = board[from.x][rook_from_y];
		board[from.x][rook_to_y]->pos.x = from.x;
		board[from.x][rook_to_y]->pos.y = rook_to_y;
		board[from.x][rook_from_y] = nullptr;
		// update board_status
		board_status[0] = 1; // castling move
		board_status[1] = 0; // not an en passant
		board_status[2] = -1; // no captured piece
		board_status[3] = 1; // rook has just moved
		board_status[4] = 1; // king has just moved
	}
	else if (from_piece->type == 0 && to.y != from.y && board[to.x][to.y] == nullptr) { // en passant
		board[to.x][to.y] = board[from.x][from.y];
		board[to.x][to.y]->pos.x = to.x;
		board[to.x][to.y]->pos.y = to.y;
		board[from.x][from.y] = nullptr;
		board[from.x][to.y] = nullptr;
		// update board_status
		board_status[0] = 0; // not a castling move
		board_status[1] = 1; // en passant move
		board_status[2] = 0; // pawn captured necessarily
		board_status[3] = 0; // no change to rooks
		board_status[4] = 0; // no change to king
	}
	else { // normal move
		Piece* captured = board[to.x][to.y];
  		board[to.x][to.y] = board[from.x][from.y];
  		board[to.x][to.y]->pos.x = to.x;
  		board[to.x][to.y]->pos.y = to.y;
  		board[from.x][from.y] = nullptr;
		
		if (promotion > -1) {
			if (promotion == 1) board[to.x][to.y] = new Knight{color, Pos{to.x,to.y}};
			if (promotion == 2) board[to.x][to.y] = new Bishop{color, Pos{to.x,to.y}};
			if (promotion == 3) board[to.x][to.y] = new Rook{color, Pos{to.x,to.y}};
			if (promotion == 4) board[to.x][to.y] = new Queen{color, Pos{to.x, to.y}};
		}

		// update board_status
		board_status[0] = 0; // not castling
		board_status[1] = 0; // not en passant
		board_status[2] = (captured == nullptr)?-1:captured->type;
		if (board[to.x][to.y]->type == 3) {board_status[3] = (board[to.x][to.y]->has_moved)?0:1; board[to.x][to.y]->has_moved=true;} // normal rook move
		else board_status[3] = 0;
		if (board[to.x][to.y]->type == 5) {board_status[4] = (board[to.x][to.y]->has_moved)?0:1; board[to.x][to.y]->has_moved=true;} // normal king move
		else board_status[4] = 0;
		if (board[to.x][to.y]->type == 0 && abs(to.x-from.x)==2) {
			// std::cout << move_str << std::endl;
			// std::cout << to.x << to.y << std::endl; 
			board[to.x][to.y]->just_moved = true;} // double pawn move
		
		/*
		if (captured!=nullptr && captured->type == 0 && captured->just_moved) board_status[6] = 1; // test
		else board_status[6] = 0;*/
	}
	
	if (board_status[5] > -1) { // remove just_moved status
		Piece* pawn = board[p_line][board_status[5]];
		if (pawn!=nullptr && pawn->type == 0 && pawn->color != color)
			pawn->just_moved = false;
	}

}

void Board::undo_move(std::string move_str, int* board_status) {
	Pos from = str_from(move_str);
	Pos to = str_to(move_str);
	Piece* to_piece = board[to.x][to.y];
	int color = to_piece->color;
	
	// undo a double pawn move essentially
	if (to_piece != nullptr && to_piece->type == 0 && to_piece->just_moved) to_piece->just_moved = false;

	if (board_status[0] == 1) { // undo castling
		board[from.x][from.y] = board[to.x][to.y];
		board[from.x][from.y]->pos.x = from.x;
		board[from.x][from.y]->pos.y = from.y;
		board[to.x][to.y] = nullptr;
		int rook_from_y = (to.y==2) ? 0 : 7;
		int rook_to_y = (to.y==2) ? 3 : 5;
		board[from.x][rook_from_y] = board[from.x][rook_to_y];
		board[from.x][rook_from_y]->pos.x = from.x;
		board[from.x][rook_from_y]->pos.y = rook_from_y;
		board[from.x][rook_to_y] = nullptr;
		  
		board[from.x][from.y]->has_moved = false;
		board[from.x][rook_from_y]->has_moved = false;			
	}
	else if (board_status[1] == 1) { // undo en passant
		board[from.x][from.y] = board[to.x][to.y];
  		board[from.x][from.y]->pos.x = from.x;
  		board[from.x][from.y]->pos.y = from.y;
  		board[from.x][to.y] = new Pawn{1-color, Pos{from.x,to.y}};
  		board[to.x][to.y] = nullptr;
		// board[from.x][to.y]->just_moved = true;	
	}
	else {
  		board[from.x][from.y] = board[to.x][to.y];
		if (board_status[6] > -1) // undo promotion
			board[from.x][from.y] = new Pawn{color, Pos{from.x,from.y}}; // restore pawn
  		board[from.x][from.y]->pos.x = from.x;
  		board[from.x][from.y]->pos.y = from.y;
		
		int c = board_status[2]; // restore captured piece
		if (c == 0) {board[to.x][to.y] = new Pawn{1-color,Pos{to.x,to.y}};}
		else if (c == 1) board[to.x][to.y] = new Knight{1-color,Pos{to.x,to.y}};
		else if (c==2) board[to.x][to.y] = new Bishop{1-color,Pos{to.x,to.y}};
		else if (c==3) board[to.x][to.y] = new Rook{1-color,Pos{to.x,to.y}};
		else if (c==4) board[to.x][to.y] = new Queen{1-color,Pos{to.x,to.y}};
		else board[to.x][to.y] = nullptr;
		
		if (board_status[3] == 1) board[from.x][from.y]->has_moved = false; // restore rooks status
		if (board_status[4] == 1) board[from.x][from.y]->has_moved = false; // restore kings status
	}


	if (board_status[5] > -1) { // restore pawn status
		int color = board[from.x][from.y]->color;
		int my_p_line = (color)?4:3;
		board[my_p_line][board_status[5]]->just_moved = true;
	}
}

bool Board::in_check(int color) {
  Pos k_pos = find_king(color);
  //for all opponent pieces, check if they can move to our king
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      if(board[i][j] != nullptr && board[i][j]->color != color)
        for(auto &elem : board[i][j]->legal_moves(board))
          if(elem[2] == k_pos.to_str()[0] && elem[3] == k_pos.to_str()[1])
            return true;
  return false;
}


std::vector<std::string> Board::all_legal_moves(int color) {
  if(color != 0 && color != 1) {
    Util::exitError("Unrecognized color: " + std::to_string(color));
  }
  std::vector<std::string> cache;
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      //for all positions occupied by the color
      if(board[i][j] != nullptr && board[i][j]->color == color) {
        // std::cout << i << " " << j << std::endl;

        //we can take all (normal) legal moves with the function
        std::vector<std::string> store = board[i][j]->legal_moves(board);
        for(auto &str : store) {
	  int board_status[10];
	  move(str,board_status);
          if(!in_check(color))
            cache.push_back(str);
          undo_move(str,board_status);
        }
      }

  // check castling:
  int kx = color ? 0 : 7;
  int ky = 4;
  if (board[kx][ky] != nullptr && board[kx][ky]->type == 5 && board[kx][ky]->color == color && !board[kx][ky]->has_moved) {
	if (board[kx][0] != nullptr && board[kx][0]->type == 3 && board[kx][0]->color == color && !board[kx][0]->has_moved)
		if ( board[kx][1] == nullptr && board[kx][2] == nullptr && board[kx][3] == nullptr ) {
			bool can_castle_l = true;
			int board_status1[6], board_status2[6];
			std::string move_str1 =Pos{kx,4}.to_str() + Pos{kx,3}.to_str(); 
			move(move_str1, board_status1);
			if (in_check(color)) can_castle_l = false;
			std::string move_str2 = Pos{kx,3}.to_str() + Pos{kx,2}.to_str();
			move(move_str2, board_status2);
			if (in_check(color)) can_castle_l = false;
			undo_move(move_str2, board_status2);
			undo_move(move_str1, board_status1);
			if (can_castle_l) cache.push_back(Pos{kx,4}.to_str() + Pos{kx,2}.to_str());
		}
	if (board[kx][7] != nullptr && board[kx][7]->type == 3 && board[kx][7]->color == color && !board[kx][7]->has_moved)
		if ( board[kx][5] == nullptr && board[kx][6] == nullptr ) {
			bool can_castle_s = true;
			int board_status1[6], board_status2[6];
			std::string move_str1 =Pos{kx,4}.to_str()+Pos{kx,5}.to_str();
			std::string move_str2 = Pos{kx,5}.to_str()+Pos{kx,6}.to_str();
			move(move_str1, board_status1);
			if (in_check(color)) can_castle_s = false;
			move(move_str2, board_status2);
			if (in_check(color)) can_castle_s = false;
			undo_move(move_str2, board_status2);
			undo_move(move_str1, board_status1);
			if (can_castle_s) cache.push_back(Pos{kx,4}.to_str()+Pos{kx,6}.to_str());
		}
  }
  
  // check en passant:
  int px = color ? 4 : 3; 
  for ( int j = 0; j < 8; j++ )
	if ( board[px][j] != nullptr && board[px][j]->type == 0 && board[px][j]->color == color ) {
		if (j-1 >= 0 && board[px][j-1] != nullptr && board[px][j-1]->type == 0 && board[px][j-1]->just_moved && board[(color?px+1:px-1)][j-1]==nullptr) {
			int board_status[10];
			std::string move_str =Pos{px,j}.to_str()+Pos{(color?px+1:px-1),j-1}.to_str();  
		  	move(move_str, board_status);	
			if (!in_check(color)) {
				cache.push_back(Pos{px,j}.to_str()+Pos{(color?px+1:px-1),j-1}.to_str());
				//display();
				//std::cout << px << " " << j-1 << std::endl;
			}
			undo_move(move_str, board_status);
			/*display();
			std::cout << "ABOVE IS REVERSED" << std::endl;
			std::cout << board[px][j-1]->just_moved << std::endl;*/
		}
		if (j+1 < 8 && board[px][j+1] != nullptr && board[px][j+1]->type == 0 && board[px][j+1]->just_moved && board[(color?px+1:px-1)][j+1]==nullptr) {
			int board_status[10];
			std::string move_str =Pos{px,j}.to_str()+Pos{(color?px+1:px-1),j+1}.to_str();
		      	move(move_str, board_status);	
			if (!in_check(color)) {
				cache.push_back(Pos{px,j}.to_str()+Pos{(color?px+1:px-1),j+1}.to_str());
				//display();	
				//std::cout << px << " " << j+1 << std::endl;
			}
			undo_move(move_str, board_status);
			/*display();
			std::cout << "ABOVE IS REVERSED" << std::endl;
			std::cout << board[px][j+1]->just_moved << std::endl;*/
		}
	}
 
  return cache;
}

/*
 * Below are done
 *
 */
void Board::init() {
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++) {
      Pos p{i, j};
      board[i][j] = nullptr;
    }

  // init kings
  Pos wkp{0, 4};
  Pos bkp{7, 4};
  board[0][4] = new King{1, wkp};
  board[7][4] = new King{0, bkp};

  // init pawns
  for(int j = 0; j < 8; j++) {
    Pos wpp{1, j};
    Pos bpp{6, j};
    board[1][j] = new Pawn{1, wpp};
    board[6][j] = new Pawn{0, bpp};
  }

  // init knights
  Pos wnp1{0, 1};
  Pos wnp2{0, 6};
  Pos bnp1{7, 1};
  Pos bnp2{7, 6};
  board[0][1] = new Knight{1, wnp1};
  board[0][6] = new Knight{1, wnp2};
  board[7][1] = new Knight{0, bnp1};
  board[7][6] = new Knight{0, bnp2};

  // init bishops
  Pos wbp1{0, 2};
  Pos wbp2{0, 5};
  Pos bbp1{7, 2};
  Pos bbp2{7, 5};
  board[0][2] = new Bishop{1, wbp1};
  board[0][5] = new Bishop{1, wbp2};
  board[7][2] = new Bishop{0, bbp1};
  board[7][5] = new Bishop{0, bbp2};

  // init rooks
  Pos wrp1{0, 0};
  Pos wrp2{0, 7};
  Pos brp1{7, 0};
  Pos brp2{7, 7};
  board[0][0] = new Rook{1, wrp1};
  board[0][7] = new Rook{1, wrp2};
  board[7][0] = new Rook{0, brp1};
  board[7][7] = new Rook{0, brp2};

  // init queens
  Pos wqp{0, 3};
  Pos bqp{7, 3};
  board[0][3] = new Queen{1, wqp};
  board[7][3] = new Queen{0, bqp};
}

void Board::display() {
  std::string line = "  ";
  for(int i = 0; i < 8; i++)
    line += " ----";
  line += "\n";

  std::string out = line;
  for(int i = 7; i >= 0; i--) {
    char row = ('1' + i);
    std::string row1{row};
    out += row1 + " ";
    for(int j = 0; j < 8; j++) {
      out += "| ";
      Piece *p = board[i][j];
      if(p == nullptr)
        out += "   ";
      else {
        if(p->color == 0)
          out += "b";
        else
          out += "w";
        if(p->type == 0)
          out += "P ";
        if(p->type == 1)
          out += "N ";
        if(p->type == 2)
          out += "B ";
        if(p->type == 3)
          out += "R ";
        if(p->type == 4)
          out += "Q ";
        if(p->type == 5)
          out += "K ";
      }
    }
    out += "|\n" + line;
  }
  out += "    a    b    c    d    e    f    g    h";
  std::cout << out << std::endl;
}
