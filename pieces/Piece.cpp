#include "Piece.h"

void Piece::set_pos(Pos pos) {
  this->pos = pos;
}

Pos Piece::get_pos() {
  return pos;
}

int Piece::get_color() {
  return color;
}

int Piece::get_type() {
  return type;
}
