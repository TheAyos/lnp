#include "Game.h"
#include "Parser.h"
#include "pieces/Piece.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
int main(int argc, char **argv) {

  srand(time(NULL));
  // TODO: all use same clang-format style
  
  Game Chess;
  // std::cout << Chess.search(4,1) << std::endl;

  Parser parser{argc, argv};
  // std::cout << "parsing" << std::endl;
  parser.parseArgs();
  // std::cout << "arguments received" << std::endl;
  int turn = parser.parseHistory(&Chess.my_board);

  // Chess.my_board.display();

  std::cout << "TURN FOR " << turn << std::endl;
  std::vector<std::string> moves = Chess.my_board.all_legal_moves(turn);
  
  // for (auto & element : moves) std::cout << element << std::endl;
  
  // moves = Chess.my_board.all_legal_moves(1);
  // for (auto&element:moves) std::cout << element << std::endl;
  // std::cout << Chess.search(4,1) << std::endl;
  int rand_int = rand() % moves.size();

  double db = Chess.search_best(3, turn, turn?-9999.0:9999.0);

  std::cout << Chess.my_move << std::endl;
  // std::cout << moves[rand_int] << std::endl;
  // parser.writeNextMove(moves[rand_int]);
  if (Chess.my_move == "")
	parser.writeNextMove(moves[0]);
  else
  	parser.writeNextMove(Chess.my_move);
  Chess.my_board.display();
 
  /*
  Game Chess1;
  Board pos3 = Chess1.my_board;
  for (int i = 0; i < 8; i++)
	  for (int j = 0; j < 8; j++)
		  pos3.board[i][j] = nullptr;
  pos3.board[1][4] = new Pawn{1, Pos{1,4}};
  pos3.board[1][6] = new Pawn{1, Pos{1,6}};
  pos3.board[3][1] = new Rook{1, Pos{3,1}};
  pos3.board[3][5] = new Pawn{0, Pos{3,5}};
  pos3.board[3][7] = new King{0, Pos{3,7}};
  pos3.board[4][0] = new King{1, Pos{4,0}};
  pos3.board[4][1] = new Pawn{1, Pos{4,1}};
  pos3.board[4][7] = new Rook{0, Pos{4,7}};
  pos3.board[5][3] = new Pawn{0, Pos{5,3}};
  pos3.board[6][2] = new Pawn{0, Pos{6,2}};
  // pos3.move(Pos{1,4},Pos{3,4});
  
  pos3.display();
  Chess1.my_board = pos3;
  // std::cout << Chess1.search(5,1) << std::endl;
  // std::vector<std::string> moves = pos3.all_legal_moves(1);
  for (auto & str: moves)
	  std::cout << str << std::endl;
  //std::cout << Chess1.search(4,1) << std::endl;
  */
  return 0;
}
