#include "engine/Move.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ROUND TRIP UCI", "[move]") {
  chess::engine::Move m = chess::engine::Move::fromUci("e2e4");
  REQUIRE(m.toUci() == "e2e4");
  chess::engine::Move promo = chess::engine::Move::fromUci("a7a8q");
  REQUIRE(promo.isPromotion());
  REQUIRE(promo.toUci() == "a7a8q");
}
