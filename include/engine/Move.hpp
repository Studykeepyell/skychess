#pragma once
#include "Square.hpp"
#include <cstdint>
#include <string>

namespace chess::engine {

enum class PieceType : uint8_t {
  King,
  Queen,
  Rook,
  Bishop,
  Knight,
  Pawn,
  None
};

enum class Color : uint8_t { White, Black, None };

enum MoveFlag : uint8_t {
  Quiet = 0,
  Capture = 1 << 0,
  Enpassent = 1 << 1,
  KingSideCastle = 1 << 2,
  QueenSideCastle = 1 << 3,
  Promotion = 1 << 4
};

struct Move {
  Square from{};
  Square to{};
  PieceType promo = PieceType::None;
  uint8_t flags = MoveFlag::Quiet;

  constexpr bool isCapture() const { return flags & MoveFlag::Capture; }
  constexpr bool isEnpassent() const { return flags & MoveFlag::Enpassent; }
  constexpr bool isCastle() const {
    // mask‐off both bits, then test if *any* of them is set
    return (flags & (MoveFlag::KingSideCastle | MoveFlag::QueenSideCastle)) !=
           0;
  }
  constexpr bool isPromotion() const { return flags & MoveFlag::Promotion; }

  /*uci helper (e2e4 ,  e7e8q)*/
  std::string toUci() const;
  static Move fromUci(const std::string &str);
  /*simple equality so std::find works on std::vector<Move>*/
  bool operator==(const Move &) const = default;
}; // namespace chess::engine

} // namespace chess::engine
