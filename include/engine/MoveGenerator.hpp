#pragma once
#include "Board.hpp"
#include "Move.hpp"
#include <vector>

namespace chess::engine {

/**
 * @brief stateless utility that returns all pseudo-legal moves
 *     for the current position. Legality-flitering (king-in-check)
 *     is done inside generateLegal ().
 */

class MoveGenerator {
public:
  static std::vector<Move> generatePseudoLegal(const Board &);
  static std::vector<Move> generateLegal(const Board &);

private:
  /* Per-piece helpers (each pushes into the vector) */
  static void genPawn(const Board &, Square, std::vector<Move> &);
  static void genKnight(const Board &, Square, std::vector<Move> &);
  static void genBishop(const Board &, Square, std::vector<Move> &);
  static void genQueen(const Board &, Square, std::vector<Move> &);
  static void genKing(const Board &, Square, std::vector<Move> &);
  static void genRook(const Board &, Square, std::vector<Move> &);
  static bool isSquareAttacked(const Board &, Square, Color by);
};
} // namespace chess::engine
