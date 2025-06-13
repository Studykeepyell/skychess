#include "engine/Board.hpp"
#include <catch2/catch_test_macros.hpp> // ✅ Catch2 v3-style

TEST_CASE("Intitial Board have 32 Pieces", "[board]") {
  chess::engine::Board b;
  int count = 0;
  for (auto p : b.pieces())
    if (p != chess::engine::PieceType::None)
      ++count;

  REQUIRE(count == 32);
}

TEST_CASE("King Square start at e1 , e8", "[board]") {
  chess::engine::Board b;
  using chess::engine::Color;
  REQUIRE(b.kingSquare(Color::White) == 4);
  REQUIRE(b.kingSquare(Color::Black) == 60);
}
