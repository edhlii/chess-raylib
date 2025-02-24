#pragma once

#include "board.h"
#include "common.h"
#include "move.h"
#include <raylib.h>
#include <vector>

class Game {
public:
  Game();
  void Run();
  void HandleInput();
  void GetValidMoves(Position pos);
  bool IsFriendlyColor(int fPiece, int sPiece);
  bool IsOutOfBound(Position pos);
  bool SearchValidMove(Move move);
  void GetSlidingMoves(int piece, Position pos);
  void GetKnightMoves(int piece, Position pos);
  void GetKingMoves(int piece, Position pos);
  void GetPawnMoves(int piece, Position pos);
  bool CanEnPassant(int piece, Position pos);
  void Draw();

private:
  std::vector<Move> moves;
  Board *board;
  bool isPieceSelected = false;   // Whether a piece is selected
  int selectedPiece = NONE;       // The selected piece
  int selectedFile, selectedRank; // Original position
};
