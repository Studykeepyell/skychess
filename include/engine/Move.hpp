#pragma once
#include "Square.hpp"

namespace chess::engine {

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn, None };

enum class Color { White, Black };

struct Move {
  Square from{}, to{};
  PieceType promotion{PieceType::None};
  bool isEnPassant{false};
  bool isCastle{false};
};
}; // namespace chess::engine
// namespace chess::engine
