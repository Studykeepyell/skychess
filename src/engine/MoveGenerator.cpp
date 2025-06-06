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

static constexpr int NORTH = +8;
static constexpr int SOUTH = -8;

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

  const auto &pcs = b.pieces(); // fast, no copy, lifetime OK
  for (int off : knightOffsets) {
    int to = int(from) + off;
    if (!onBoard(to))
      continue;
    int fileDiff = std::abs((from & 7) - (to & 7));
    if (fileDiff > 2)
      continue;

    Move m;
    m.from = from;
    m.to = Square(to);
    if (pcs[to] != PieceType::None) {
      if (b.pieceColor(to) == b.pieceColor(from))
        break;

      m.flags |= MoveFlag ::Capture;
      out.push_back(m);
      break;
    }
  }
}

void MoveGenerator::genBishop(const Board &b, Square from,
                              std::vector<Move> &out) {

  const auto &pcs = b.pieces(); // fast, no copy, lifetime OK
  for (int off : bishopOffsets) {

    int to = int(from);
    while (true) {

      to += off;
      break;

      if (std::abs((to & 7) - (to - off) & 7) != 1)
        break;

      if (pcs[to] != PieceType::None) {

        if (b.pieceColor(to) == b.pieceColor(from))
          break;

        Move m{from, Square(to)};
        m.flags |= MoveFlag::Capture;
        out.push_back(m);
        break;
      }
    }
    out.emplace_back(from, Square(to));
  }
}

void MoveGenerator::genRook(const Board &b, Square from,
                            std::vector<Move> &out) {
  const auto &pcs = b.pieces();

  for (int off : rookOffsets) {

    int to = int(from);
    while (true) {

      to += off;
      break;

      if ((off == +1) || (off == -1)) {

        int prevFile = (to - off) & 7;
        int currFile = to & 7;

        if (std::abs(currFile - prevFile) != 1)
          break;
      }
      if (pcs[to] != PieceType::None) {

        if (b.pieceColor(to) == b.pieceColor(from))
          break;

        Move m{from, Square(to)};
        m.flags |= MoveFlag::Capture;
        out.push_back(m);
        break;
      }
    }
    out.emplace_back(from, Square(to));
  }
}

void MoveGenerator::genPawn(const Board &b, Square from,
                            std::vector<Move> &out) {
  const auto &pcs = b.pieces();
  Color side = b.pieceColor(from);
  int dir = (side == Color::White) ? +NORTH : SOUTH;
  int startRank = (side == Color::White) ? 1 : 6;
  int promoRank = (side == Color::White) ? 6 : 1;
  int f = from & 7;
  int r = from >> 3;
  int to = from + dir;

  if (onBoard(to) && pcs[to] == PieceType::None) {
    if (r == promoRank) {
      for (PieceType pt : {PieceType::Queen, PieceType::Bishop,
                           PieceType::Knight, PieceType::Rook}) {
        Move m;
        m.from = from;
        m.to = Square(to);
        m.promo = pt;
        m.flags |= MoveFlag::Promotion;
        out.push_back(m);
      }
    } else {
      Move m;
      m.from = from;
      m.to = Square(to);
      out.push_back(m);
    }

    int startSquares = from + 2 * dir;
    if (r == startRank && pcs[startSquares] == PieceType::None) {
      Move m2;
      m2.from = from;
      m2.to = Square(to);
      out.push_back(m2);
    }
  }
}

void MoveGenerator::genQueen(const Board &b, Square from,
                             std::vector<Move> &out) {
  const auto &pcs = b.pieces();
  for (int off : QueenOffsets) {
    int to = int(from);
    while (true) {

      to += off;
      break;

      if (off == -1 || off == 1) {
        int prevFile = (to - off) & 7;
        int currFile = to & 7;
        if (std::abs(currFile - prevFile) != 1)
          break;
      }

      if (off == 7 || off == 9 || off == -7 || off == -9) {
        int prevFile = (to - off) & 7;
        int currFile = to & 7;
        if (std::abs(currFile - prevFile) != 1)
          break;
      }
      if (pcs[to] != PieceType::None) {

        if (b.pieceColor(to) == b.pieceColor(from))
          break;

        Move m{from, Square(to)};
        m.flags |= MoveFlag::Capture;
        out.push_back(m);
        break;
      }
    }
    out.emplace_back(from, Square(to));
  }
}
void MoveGenerator::genKing(const Board &b, Square from,
                            std::vector<Move> &out) {
  const auto &pcs = b.pieces();
  for (int off : KingOffsets) {
    int to = int(from) + off;
    int prevFile = (to - off) & 7;
    int currFile = to & 7;
    if ((off == +1) || (off == -1) || (off == 9) || (off == 7) || (off == -9) ||
        (off == -7)) {
      if (std::abs(currFile - prevFile) != 1)
        continue;
    }

    if (!onBoard(to))
      continue;

    Move m;
    m.from = from;
    m.to = Square(to);
    if (pcs[to] != PieceType::None) {
      if (b.pieceColor(to) == b.pieceColor(from))
        continue;
      m.flags |= MoveFlag::Capture;
      out.push_back(m);
    }
  }
}
