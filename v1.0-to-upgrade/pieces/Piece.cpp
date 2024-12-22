#include "Piece.h"


std::string Pos::to_str() {
	std::string out = "";
	out += ('a'+y);
	out += ('1'+x);
	return out;
}

bool Pos::in_bound() {
	return (x < 8 && x >= 0 && y < 8 && y >= 0);
}

bool Pos::operator==(const Pos& other) const {
    return x == other.x && y == other.y;
}

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
