#pragma once
#include <cstdint>

namespace chess::engine {

// 0 = a1, 7 = h1, 56 = a8, 63 = h8   (rank * 8 + file)
using Square = std::uint8_t;

/*  lowest 3 bits  → file  (0-7)  */
constexpr int file(Square s) noexcept { return s & 7; }

/*  shift away the file bits → rank (0-7)  */
constexpr int rank(Square s) noexcept { return s >> 3; }

/*  optional helper to build a square from (file,rank) */
constexpr Square makeSquare(int file, int rank) noexcept {
  return static_cast<Square>(rank * 8 + file); // 0–63
}

} // namespace chess::engine
