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

Piece *Board::move(Pos from, Pos to) {
  Piece *captured = board[to.x][to.y];
  board[to.x][to.y] = board[from.x][from.y];
  board[to.x][to.y]->pos.x = to.x;
  board[to.x][to.y]->pos.y = to.y;
  board[from.x][from.y] = nullptr;
  return captured;
}

void Board::undo_move(Pos from, Pos to, Piece *captured) {
  board[from.x][from.y] = board[to.x][to.y];
  board[from.x][from.y]->pos.x = from.x;
  board[from.x][from.y]->pos.y = from.y;
  board[to.x][to.y] = captured;
}

bool Board::in_check(int color) {
  Pos k_pos{0, 0};
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      if(board[i][j] != nullptr && board[i][j]->type == 5
         && board[i][j]->color == color)
        k_pos = board[i][j]->pos;

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
      if(board[i][j] != nullptr && board[i][j]->color == color) {
        // std::cout << i << " " << j << std::endl;
        std::vector<std::string> store = board[i][j]->legal_moves(board);
        for(auto &str : store) {
          // std::cout << str << std::endl;
          Pos from = str_from(str);
          Pos to = str_to(str);
          Piece *c = move(from, to);
          if(!in_check(color))
            cache.push_back(str);
          undo_move(from, to, c);
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
