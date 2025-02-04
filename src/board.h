#pragma once
#include "common.h"
#include <map>
#include <raylib.h>
#include <string>

class Board {
public:
  Board();
  void Initialize();
  void InitializeTextures();
  bool IsTurn(int piece);
  void LoadFenPosition(std::string fen);
  int GetPieceType(int piece);
  int GetPieceColor(int piece);
  void DrawBoard();
  void DrawPiece();
  void DrawSinglePiece(int piece, int file, int rank);
  int grid[8][8];
  bool isWhiteTurn = true;

private:
  std::map<int, Texture2D> pieceTextures;
};
