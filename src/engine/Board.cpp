#include "../../include/engine/Board.hpp"
#include <stdexcept>

using namespace chess::engine;

namespace {

void placeBackRank(std::array<PieceType, 64> &s, int rank, bool white) {
  int r = white ? rank : (7 - rank);
  s[r * 8 + 0] = PieceType ::Rook;
  s[r * 8 + 1] = PieceType ::Knight;
  s[r * 8 + 2] = PieceType ::Bishop;
  s[r * 8 + 3] = PieceType ::Queen;
  s[r * 8 + 4] = PieceType ::King;
  s[r * 8 + 5] = PieceType ::Bishop;
  s[r * 8 + 6] = PieceType ::Knight;
  s[r * 8 + 7] = PieceType ::Rook;
}
} // namespace

Board::Board() {
  squares_.fill(PieceType::None);
  placeBackRank(squares_, 0, true);
  placeBackRank(squares_, 7, true);

  // pawns
  for (int i = 0; i < 8; ++i) {
    squares_[8 + i] = PieceType::Pawn;
  }
  for (int i = 0; i < 8; ++i) {
    squares_[6 * 8 + i] = PieceType::Pawn;
  }
}

Board::Board(const std::string & /*fen*/) : Board() { /* TODO: parse FEN */ }

std::vector<Move> Board::legalMoves() const { return {}; }

bool Board::makeMove(const Move &m) {
  squares_[m.to] = squares_[m.from];
  squares_[m.from] = PieceType::None;
  stm_ = (stm_ == Color::White) ? Color::Black : Color::White;
  return true;
}
