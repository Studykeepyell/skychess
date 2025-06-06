#include "../../include/engine/Board.hpp"
#include <stdexcept>
#include <string>
#include <vector>

using namespace chess::engine;

namespace {

void placeBackRank(std::array<PieceType, 64> &s, std::array<Color, 64> &c,
                   int rank, Color side) {
  int r = (side == Color::White) ? rank : (7 - rank);
  int base = r * 8;

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
  squares_.fill(PieceType::None);
  colours_.fill(Color::None);

  placeBackRank(squares_, colours_, 0, Color::White);

  placeBackRank(squares_, colours_, 7, Color::Black);

  // pawns
  for (int i = 0; i < 8; ++i) {
    squares_[8 + i] = PieceType::Pawn;
    colours_[8 + i] = Color::White;
  }
  for (int i = 0; i < 8; ++i) {
    squares_[6 * 8 + i] = PieceType::Pawn;
    colours_[8 + i] = Color::Black;
  }
}

Board::Board(const std::string & /*fen*/) : Board() { /* TODO: parse FEN */ }

std::vector<Move> Board::legalMoves() const { return {}; }

bool Board::makeMove(const Move &m) {
  squares_[m.to] = squares_[m.from];
  colours_[m.to] = colours_[m.from];
  squares_[m.from] = PieceType::None;
  colours_[m.from] = Color::None;
  stm_ = (stm_ == Color::White) ? Color::Black : Color::White;
  return true;
}
