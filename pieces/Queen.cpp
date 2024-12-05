#include "Queen.h"
#include <cmath>

Queen::Queen(int color, Pos pos) {
    this->type = 4;
    this->color = color;
    this->pos = pos;
}

bool Queen::potential_move(Pos to) {
    int dx = std::abs(to.x - pos.x);
    int dy = std::abs(to.y - pos.y);

    return (to.x == pos.x || to.y == pos.y || dx == dy);
}