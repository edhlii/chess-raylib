#include "game.h"
#include "common.h"
#include <iostream>
#include <raylib.h>

Game::Game() {
  SetTargetFPS(60);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chess Raylib");
  board = new Board();
}

bool Game::IsFriendlyColor(int fPiece, int sPiece) {
  return board->GetPieceColor(fPiece) == board->GetPieceColor(sPiece);
}

void Game::GetSlidingMoves(int piece, Position pos) {
  Position offset[8] = {{0, 1},  {1, 0}, {0, -1}, {-1, 0},
                        {-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
  int startIdx, endIdx;
  int pieceType = board->GetPieceType(piece);

  if (pieceType == ROOK) {
    startIdx = 0;
    endIdx = 3;
  } else if (pieceType == BISHOP) {
    startIdx = 4;
    endIdx = 7;
  } else { // Queen
    startIdx = 0;
    endIdx = 7;
  }
  Position target;
  int targetPiece;
  for (int idx = startIdx; idx <= endIdx; idx++) {
    for (int n = 1; n <= 8; n++) {
      target.file = pos.file + offset[idx].file * n;
      target.rank = pos.rank + offset[idx].rank * n;

      if (IsOutOfBound(target))
        break;

      targetPiece = board->grid[target.rank][target.file];

      if (IsFriendlyColor(piece, targetPiece))
        break;

      moves.push_back({pos, target});

      if (targetPiece != NONE)
        break;
    }
  }
}

void Game::GetKnightMoves(int piece, Position pos) {
  Position offsets[8] = {{1, 2},  {2, 1},  {1, -2},  {2, -1},
                         {-1, 2}, {-2, 1}, {-1, -2}, {-2, -1}};
  Position target;
  int targetPiece;
  for (auto &offset : offsets) {
    target.file = pos.file + offset.file;
    target.rank = pos.rank + offset.rank;
    if (IsOutOfBound(target))
      continue;
    targetPiece = board->grid[target.rank][target.file];
    if (IsFriendlyColor(piece, targetPiece))
      continue;
    moves.push_back({pos, target});
  }
}

void Game::GetKingMoves(int piece, Position pos) {
  Position offsets[8] = {{0, 1},  {1, 0}, {0, -1}, {-1, 0},
                         {-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
  Position target;
  int targetPiece;
  for (auto &offset : offsets) {
    target.file = pos.file + offset.file;
    target.rank = pos.rank + offset.rank;
    if (IsOutOfBound(target))
      continue;
    targetPiece = board->grid[target.rank][target.file];
    if (IsFriendlyColor(piece, targetPiece))
      continue;
    moves.push_back({pos, target});
  }
}

void Game::GetPawnMoves(int piece, Position pos) {
  int pieceColor = board->GetPieceColor(piece);
  int direction = pieceColor == P_WHITE ? 1 : -1;

  Position target = {pos.file, pos.rank + direction};
  int targetPiece = board->grid[target.rank][target.file];

  if (!IsOutOfBound(target) && targetPiece == NONE) {
    moves.push_back({pos, target});
    // Move two squares forward from the starting rank
    if ((pieceColor == P_WHITE && pos.rank == 1) ||
        (pieceColor == P_BLACK && pos.rank == 6)) {
      target = {pos.file, pos.rank + 2 * direction};
      targetPiece = board->grid[target.rank][target.file];
      if (targetPiece == NONE) {
        moves.push_back({pos, target});
      }
    }
  }
  for (int dx : {-1, 1}) {
    target = {pos.file + dx, pos.rank + direction};
    if (!IsOutOfBound(target)) {
      int pieceAtTarget = board->grid[target.rank][target.file];
      if (pieceAtTarget != NONE && !IsFriendlyColor(piece, pieceAtTarget)) {
        moves.push_back({pos, target});
      }
    }
  }
}

bool Game::IsOutOfBound(Position pos) {
  if (pos.file < 0 || pos.file > 7 || pos.rank < 0 || pos.rank > 7)
    return true;
  return false;
}

void Game::GetValidMoves(Position pos) {
  int file = pos.file;
  int rank = pos.rank;
  int piece = board->grid[rank][file];
  int pieceType = board->GetPieceType(piece);
  if (piece == NONE) {
    return;
  }
  if ((pieceType == ROOK) || (pieceType == BISHOP) || (pieceType == QUEEN)) {
    GetSlidingMoves(piece, pos);
  }
  if (pieceType == KNIGHT) {
    GetKnightMoves(piece, pos);
  }
  if (pieceType == KING) {
    GetKingMoves(piece, pos);
  }
  if (pieceType == PAWN) {
    GetPawnMoves(piece, pos);
  }
}

bool Game::SearchValidMove(Move move) {
  for (auto &it : moves) {
    if (move == it)
      return true;
  }
  return false;
}

void Game::HandleInput() {
  Vector2 mousePos = GetMousePosition();
  int file = mousePos.x / SQUARE_SIZE;
  int rank = 8 - (mousePos.y / SQUARE_SIZE); // Convert to chess coordinates
  if (IsOutOfBound({file, rank}))
    return;
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (!isPieceSelected) { // First click: Select a piece
      if (board->grid[rank][file] != NONE) {
        selectedPiece = board->grid[rank][file];
        if (!board->IsTurn(selectedPiece)) {
          selectedPiece = NONE;
          return;
        }
        isPieceSelected = true;
        selectedFile = file;
        selectedRank = rank;
        GetValidMoves({file, rank});
      }
    } else { // Second click: Move the piece
      if (rank == selectedRank && file == selectedFile) {
        isPieceSelected = false;
        selectedPiece = NONE;
        moves.clear();
        return;
      }
      // TODO: Update valid moves and delete this shiet
      if (IsFriendlyColor(selectedPiece, board->grid[rank][file])) {
        isPieceSelected = false;
        selectedPiece = NONE;
        moves.clear();
        return;
      }
      Move move = {{selectedFile, selectedRank}, {file, rank}};
      if (!SearchValidMove(move)) {
        return;
      }
      board->grid[rank][file] = selectedPiece; // Place piece
      board->grid[selectedRank][selectedFile] = NONE;
      isPieceSelected = false; // Deselect
      selectedPiece = NONE;
      board->isWhiteTurn = !board->isWhiteTurn;
      moves.clear();
    }
  }
}

void Game::Run() {
  while (!WindowShouldClose()) {

    HandleInput();
    BeginDrawing();
    {
      ClearBackground(BLACK);
      Draw();
      DrawFPS(WINDOW_WIDTH - 100, 20);
    }
    EndDrawing();
  }
  CloseWindow();
}

void Game::Draw() {
  board->DrawBoard();
  board->DrawPiece();
  // Highlight selected piece and valid moves
  if (isPieceSelected) {
    DrawRectangleLines(selectedFile * SQUARE_SIZE,
                       (7 - selectedRank) * SQUARE_SIZE, SQUARE_SIZE,
                       SQUARE_SIZE, RED);
    for (auto &move : moves) {
      int file = move.target.file;
      int rank = move.target.rank;
      // Draw valid moves
      DrawRectangle(file * SQUARE_SIZE, (7 - rank) * SQUARE_SIZE, SQUARE_SIZE,
                    SQUARE_SIZE, {255, 0, 0, 128});
    }
  }
}
