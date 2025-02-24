#pragma once
#include <map>
#include <raylib.h>
#include <string>

const int WINDOW_WIDTH = 750;
const int WINDOW_HEIGHT = 600;
const int SQUARE_SIZE = 75;

const int NONE = 0;
const int KING = 1;
const int PAWN = 2;
const int KNIGHT = 3;
const int BISHOP = 4;
const int ROOK = 5;
const int QUEEN = 6;

const int P_WHITE = 8;
const int P_BLACK = 16;

const std::string WHITE_PATH = "../assets/white/";
const std::string BLACK_PATH = "../assets/black/";

const std::map<char, int> pieceTypeFromSymbol = {{'p', PAWN},  {'n', KNIGHT},
                                                 {'r', ROOK},  {'b', BISHOP},
                                                 {'q', QUEEN}, {'k', KING}};
const std::map<int, char> pieceSymbolFromType = {{PAWN, 'p'},  {KNIGHT, 'n'},
                                                 {ROOK, 'r'},  {BISHOP, 'b'},
                                                 {QUEEN, 'q'}, {KING, 'k'}};

struct Position {
  int file;
  int rank;
  bool operator==(const Position &other) const {
    return file == other.file && rank == other.rank;
  }
};
