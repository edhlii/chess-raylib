#include "board.h"
#include "common.h"
#include <raylib.h>

Board::Board() { Initialize(); }

void Board::Initialize() {
  InitializeTextures();
  for (int file = 0; file < 8; file++) {
    for (int rank = 0; rank < 8; rank++) {
      grid[file][rank] = NONE;
    }
  }
  LoadFenPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Board::InitializeTextures() {
  pieceTextures[P_WHITE | PAWN] =
      LoadTexture((WHITE_PATH + "Pawn.png").c_str());
  pieceTextures[P_WHITE | ROOK] =
      LoadTexture((WHITE_PATH + "Rook.png").c_str());
  pieceTextures[P_WHITE | KNIGHT] =
      LoadTexture((WHITE_PATH + "Knight.png").c_str());
  pieceTextures[P_WHITE | BISHOP] =
      LoadTexture((WHITE_PATH + "Bishop.png").c_str());
  pieceTextures[P_WHITE | QUEEN] =
      LoadTexture((WHITE_PATH + "Queen.png").c_str());
  pieceTextures[P_WHITE | KING] =
      LoadTexture((WHITE_PATH + "King.png").c_str());

  pieceTextures[P_BLACK | PAWN] =
      LoadTexture((BLACK_PATH + "Pawn.png").c_str());
  pieceTextures[P_BLACK | ROOK] =
      LoadTexture((BLACK_PATH + "Rook.png").c_str());
  pieceTextures[P_BLACK | KNIGHT] =
      LoadTexture((BLACK_PATH + "Knight.png").c_str());
  pieceTextures[P_BLACK | BISHOP] =
      LoadTexture((BLACK_PATH + "Bishop.png").c_str());
  pieceTextures[P_BLACK | QUEEN] =
      LoadTexture((BLACK_PATH + "Queen.png").c_str());
  pieceTextures[P_BLACK | KING] =
      LoadTexture((BLACK_PATH + "King.png").c_str());
}

void Board::LoadFenPosition(std::string fen) {
  int file = 0, rank = 7;
  for (int idx = 0; idx < fen.size(); idx++) {
    if (fen[idx] == ' ')
      break;
    if (fen[idx] == '/') {
      file = 0;
      rank--;
    } else {
      if (fen[idx] >= '0' && fen[idx] <= '9') {
        file += (fen[idx] - '0');
      } else {
        int pieceColor, pieceType;
        if (fen[idx] >= 'A' && fen[idx] <= 'Z')
          pieceColor = P_WHITE;
        else
          pieceColor = P_BLACK;
        pieceType = pieceTypeFromSymbol.at(std::tolower(fen[idx]));
        grid[rank][file] = pieceType | pieceColor;
        file++;
      }
    }
  }
}

std::string Board::GetFenString() {
  std::string fen = "";
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file < 8; file++) {
      int piece = grid[rank][file];
      if (piece == NONE) {
        continue;
      }
      int pieceType = GetPieceType(piece);
      int pieceColor = GetPieceColor(piece);
      char pieceSymbol = pieceSymbolFromType.at(pieceType);
      if (pieceColor == P_WHITE) {
        pieceSymbol = std::toupper(pieceSymbol);
      }
      fen = fen + pieceSymbol;
    }
    fen = fen + "/";
  }
  fen.pop_back();
  return fen;
}

bool Board::IsTurn(int piece) {
  int pieceColor = piece & P_WHITE ? P_WHITE : P_BLACK;
  if (pieceColor == P_WHITE && isWhiteTurn)
    return true;
  if (pieceColor == P_BLACK && !isWhiteTurn)
    return true;
  return false;
}

void Board::DrawBoard() {
  for (int file = 0; file < 8; file++) {
    for (int rank = 0; rank < 8; rank++) {
      bool isWhiteSquare = (file + rank) % 2 == 0;
      Color squareColor = isWhiteSquare ? WHITE : BROWN;
      DrawRectangle(rank * SQUARE_SIZE, file * SQUARE_SIZE, SQUARE_SIZE,
                    SQUARE_SIZE, squareColor);
    }
  }
}

int Board::GetPieceType(int piece) { return piece & 7; }

int Board::GetPieceColor(int piece) { return piece & (P_WHITE | P_BLACK); }

void Board::DrawSinglePiece(int piece, int file, int rank) {
  if (piece == 0)
    return;
  Texture2D pieceTexture = pieceTextures[piece];
  DrawTexturePro(
      pieceTexture,
      {0.0, 0.0, (float)pieceTexture.width, (float)pieceTexture.height},
      {(float)(file)*SQUARE_SIZE, (float)(7 - rank) * SQUARE_SIZE, SQUARE_SIZE,
       SQUARE_SIZE},
      {0.0, 0.0}, 0.0, WHITE);
}

void Board::DrawPiece() {
  for (int file = 0; file < 8; file++) {
    for (int rank = 0; rank < 8; rank++) {
      int idx = rank * 8 + file;
      DrawSinglePiece(grid[rank][file], file, rank);
    }
  }
}
