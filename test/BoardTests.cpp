#include "engine/Board.hpp"
#include <catch2/catch_test_macros.hpp> // ✅ Catch2 v3-style

TEST_CASE("Intitial Board have 32 Pieces", "board") { chess::engine::Board b; }
