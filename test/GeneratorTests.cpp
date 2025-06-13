#include "engine/Board.hpp"
#include "engine/MoveGenerator.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Start Position has 20 legal moves", "[gen]") {
  chess::engine::Board b;
  auto moves = chess::engine::MoveGenerator::generatePseudoLegal(b);
  REQUIRE(moves.size() == 20);
}
