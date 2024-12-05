#include "King.h"
#include <cmath>

King::King(int color, Pos pos) {
  this->type = 5;
  this->color = color;
  this->pos = pos;
  is_castled = false;
}

bool King::potential_move(Pos to) {
  if(abs(to.x - pos.x) <= 1 && abs(to.y - pos.y) <= 1)
    return true;
  return false;
}