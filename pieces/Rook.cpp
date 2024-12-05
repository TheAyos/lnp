#include "Rook.h"

Rook::Rook(int color, Pos pos) {
    this->type = 3;
    this->color = color;
    this->pos = pos;
    this->has_moved = false;
}

bool Rook::potential_move(Pos to) {
    return (to.x == pos.x || to.y == pos.y);
}

bool Rook::can_castle() {
    return !has_moved;
}