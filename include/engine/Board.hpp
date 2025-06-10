
#pragma once
#include "Move.hpp"
#include <array>
#include <string>
#include <vector>

namespace chess::engine {

/// A minimal, non-optimised 64-element array board.
class Board {
public:
  Board(); // initial position
  explicit Board(const std::string &fen);

  constexpr Square kingSquare(Color c) const noexcept {
    return kingsq[static_cast<int>]
  }

  const std::array<PieceType, 64> &pieces() const { return squares_; }
  Color sideToMove() const { return stm_; }
  Color pieceColor(Square sq) const { return colours_[sq]; }
  std::vector<Move> legalMoves() const; // TODO  – empty for now
  bool makeMove(const Move &m);         // TODO  – always true for now

private:
  std::array<PieceType, 64> squares_{};
  std::array<Color, 64> colours_;
  Square kinSq_[2] = {4, 60};
  Color stm_{Color::White};
};

} // namespace chess::engine
