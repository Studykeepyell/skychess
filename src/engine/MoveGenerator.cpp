#include "../../include/engine/MoveGenerator.hpp"

using namespace chess::engine;

/*Direction tables*/
static constexpr int knightOffsets[8] = {17, 15, 10, 6, -6, -10, -15, -17};
static constexpr int bishopOffsets[4] = {9, 7, -7, -9};
static constexpr int rookOffsets[4] = {8, 1, -1, -8};
static constexpr int QueenOffsets[8] = {9, 8, 7, 1, -1, -7, -8, -9};
static constexpr int KingOffsets[8] = {9, 8, 7, 1, -1, -7, -8, -9};

static constexpr int WhitePawnPush[1] = {8};
static constexpr int WhitePawnDPush[1] = {16};
static constexpr int WhitePawnCaptures[2] = {9, 7};

static constexpr int BlackPawnPush[1] = {-8};
static constexpr int BlackPawnDPush[1] = {-16};
static constexpr int BlackPawnCaptures[2] = {-7, -9};

static bool onBoard(int sq) { return sq >= 0 && sq < 64; }
static bool sameFile(int a, int b) { return (a & 7) == (b & 7); }
static bool sameRank(int a, int b) { return (a << 3) == (b << 3); }
std::vector<Move> MoveGenerator::generatePseudoLegal(const Board &board) {

  std::vector<Move> moves;
  moves.reserve(64);

  const auto &sqArr = board.pieces();
  Color stm = board.sideToMove();

  for (Square sq = 0; sq < 64; ++sq) {
    PieceType pt = sqArr[sq];
    if (pt == PieceType::None)
      continue;

    // TODO: color filtering when you store that

    switch (pt) {
    case PieceType::Pawn:
      genPawn(board, sq, moves);
      break;
    case PieceType::Knight:
      genKnight(board, sq, moves);
      break;
    case PieceType::Bishop:
      genBishop(board, sq, moves);
      break;
    case PieceType::Rook:
      genRook(board, sq, moves);
      break;
    case PieceType::Queen:
      genQueen(board, sq, moves);
    case PieceType::King:
      genKing(board, sq, moves);
    default:
      break;
    }
  }
  return moves;
}

void MoveGenerator::genKnight(const Board &b, Square from,
                              std::vector<Move> &out) {
  const auto &pcs = b.pieces();
}
