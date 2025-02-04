#pragma once
#include "board.h"
#include "common.h"
#include <vector>

struct Move {
  Position start;
  Position target;
  bool operator==(const Move &other) const {
    return start == other.start && target == other.target;
  }
};

struct Offset {
  int x, y;
};
