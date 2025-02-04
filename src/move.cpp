#include "move.h"
#include "common.h"

void GetSlidingMoves(std::vector<Move> &moves, int piece, Position pos) {
  Offset offset[8] = {{0, 1},  {1, 0}, {0, -1}, {-1, 0},
                      {-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
  int startIdx, endIdx;
  if (piece & ROOK) {
    startIdx = 0;
    endIdx = 3;
  } else if (piece & BISHOP) {
    startIdx = 4;
    endIdx = 7;
  } else {
    startIdx = 0;
    endIdx = 7;
  }
  for (int idx = startIdx; idx <= endIdx; idx++) {
    for (int n = 0; n < 8; n++) {
      Position target;
      target.file = pos.file + offset[idx].y * n;
      target.rank = pos.rank + offset[idx].x * n;
    }
  }
}

std::vector<Move> GetValidMoves(const int grid[8][8]) {
  std::vector<Move> moves;
  for (int rank = 0; rank < 7; rank++) {
    for (int file = 0; file < 7; file++) {
      int piece = grid[rank][file];
      if (piece == NONE)
        continue;
      if ((piece & ROOK) || (piece & BISHOP) || (piece & QUEEN)) {
        GetSlidingMoves(moves, piece, {file, rank});
      }
      Move move;
      move.startFile = file;
      move.startRank = rank;
    }
  }
  return moves;
}
