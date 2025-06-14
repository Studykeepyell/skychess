#include "../../include/engine/Board.hpp"
#include <stdexcept>
#include <string>
#include <vector>

using namespace chess::engine;

namespace {

void placeBackRank(std::array<PieceType, 64> &s, std::array<Color, 64> &c,
                   int rank, Color side) {
  int base = rank * 8;

  const std::array<PieceType, 8> order{
      PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
      PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook};

  for (int file = 0; file < 8; ++file) {
    s[base + file] = order[file];
    c[base + file] = side;
  }
}
} // namespace

Board::Board() {

  colours_.fill(Color::None);
  squares_.fill(PieceType::None);
  placeBackRank(squares_, colours_, 0, Color::White);

  placeBackRank(squares_, colours_, 7, Color::Black);

  // pawns
  for (int i = 0; i < 8; ++i) {
    squares_[8 + i] = PieceType::Pawn;
    colours_[8 + i] = Color::White;
  }
  for (int i = 0; i < 8; ++i) {
    squares_[6 * 8 + i] = PieceType::Pawn;
    colours_[6 * 8 + i] = Color::Black;
  }
}

Board::Board(const std::string & /*fen*/) : Board() { /* TODO: parse FEN */ }

std::vector<Move> Board::legalMoves() const { return {}; }

bool Board::makeMove(const Move &m) {

  epSquare_ = 64;

  squares_[m.to] = squares_[m.from];
  colours_[m.to] = colours_[m.from];
  squares_[m.from] = PieceType::None;

  colours_[m.from] = Color::None;

  if (squares_[m.to] == PieceType::Pawn) {
    int delta = int(m.to) - int(m.from);
    if (delta == +16) {

      epSquare_ = Square(m.from + 8);
    } else if (delta == -16) {

      epSquare_ = Square(m.from - 8);
    }
  }

  stm_ = (stm_ == Color::White) ? Color::Black : Color::White;
  return true;
}
