#include "Square.h"

Piece* Square::get_piece() { return piece; }

void Square::set_piece(Piece* piece) { this->piece = piece; }

void Square::clean() {piece = nullptr;}
