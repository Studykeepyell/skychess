
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
  const std::array<PieceType, 64> &pieces() const { return squares_; }
  Color sideToMove() const { return stm_; }
  Color pieceColor(Square sq) const { return colours_[sq]; }
  std::vector<Move> legalMoves() const; // TODO  – empty for now
  bool makeMove(const Move &m);         // TODO  – always true for now
  void setStartPosition();
  uint8_t castlingRights() const { return castleRights_; }
  Square kingSquare(Color c) const { return kinSq_[static_cast<int>(c)]; }
  Square enPassantSquare() const { return epSquare_; }

private:
  std::array<PieceType, 64> squares_{};
  std::array<Color, 64> colours_;

  // raw data
  uint8_t castleRights_{};
  std::array<Square, 2> kinSq_{};
  Color stm_{Color::White};

  Square epSquare_ = 64;
  // Helpers to revoke specific rights:
  void revokeKingRights(Color side);
  void revokeRookRights(Color side, bool kingside);
  void revokeRightsOnRookCapture(Square to);

  // Top-level dispatcher that calls the above three as needed:
  void updateCastleRights(const Move &m);
  void applyCastleRookMove(const Move &m);
};

} // namespace chess::engine
