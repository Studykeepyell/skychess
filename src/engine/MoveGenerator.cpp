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
static bool sameRank(int a, int b) { return (a >> 3) == (b >> 3); }
std::vector<Move> MoveGenerator::generatePseudoLegal(const Board &board) {

  std::vector<Move> moves;
  moves.reserve(64);

  const auto &sqArr = board.pieces();
  Color stm = board.sideToMove();

  for (Square sq = 0; sq < 64; ++sq) {
    PieceType pt = sqArr[sq];
    if (pt == PieceType::None)
      continue;

    if (board.pieceColor(sq) != stm)
      continue;
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
  Color me = b.pieceColor(from);

  for (int off : knightOffsets) {
    int to = int(from) + off;

    // 1) board‐bounds check
    if (!onBoard(to))
      continue;

    // 2) file‐wrap check (knight jumps two files max)
    int fileDiff = std::abs((int(from) & 7) - (to & 7));
    if (fileDiff > 2)
      continue;

    // 3) build the move
    Move m;
    m.from = from;
    m.to = Square(to);
    m.flags = 0; // clear flags

    // 4) destination empty → quiet move
    if (pcs[to] == PieceType::None) {
      out.push_back(m);
    } else {
      // 5) occupied by friend → skip
      if (b.pieceColor(to) == me)
        continue;

      // 6) occupied by enemy → capture
      m.flags |= MoveFlag::Capture;
      out.push_back(m);
    }
  }
}
void MoveGenerator::genBishop(const Board &b, Square from,
                              std::vector<Move> &out) {

  const auto &pcs = b.pieces();
  for (int off : bishopOffsets) {
    int to = int(from);
    while (true) {
      int prev = to;
      to += off;
      if (!onBoard(to))
        break;
      if (std::abs((to & 7) - (prev & 7)) != 1)
        break;

      Move m{from, Square(to)};
      if (pcs[to] != PieceType::None) {
        if (b.pieceColor(to) == b.pieceColor(from))
          break;
        m.flags |= MoveFlag::Capture;
        out.push_back(m);
        break;
      }

      out.push_back(m);
    }
  }
}

void MoveGenerator::genRook(const Board &b, Square from,
                            std::vector<Move> &out) {
  const auto &pcs = b.pieces();

  for (int off : rookOffsets) {
    int to = int(from);
    while (true) {
      int prev = to;
      to += off;
      if (!onBoard(to))
        break;
      if ((off == +1 || off == -1) && std::abs((to & 7) - (prev & 7)) != 1)
        break;

      Move m{from, Square(to)};
      if (pcs[to] != PieceType::None) {
        if (b.pieceColor(to) == b.pieceColor(from))
          break;
        m.flags |= MoveFlag::Capture;
        out.push_back(m);
        break;
      }

      out.push_back(m);
    }
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
    // double push

    int startSquares = from + 2 * dir;
    if (r == startRank && pcs[startSquares] == PieceType::None) {
      Move m2;
      m2.from = from;
      m2.to = Square(startSquares);
      out.push_back(m2);
    }
  }

  // diagonal capture include enpassent
  for (int capOff : (side == Color::White ? std::array<int, 2>{+7, +9}
                                          : std::array<int, 2>{-7, -9})) {
    int cto = from + capOff;
    if (!onBoard(cto))
      continue;
    int fileDiff = std::abs((from & 7) - (cto & 7));
    if (fileDiff != 1)
      continue;

    bool isCapture = (pcs[cto] != PieceType::None && b.pieceColor(cto) != side);

    bool isEnPass = (cto == b.enPassantSquare()); // engine-specific

    if (isCapture || isEnPass) {
      if (r == promoRank) {
        for (PieceType pt : {PieceType::Queen, PieceType::Rook,
                             PieceType::Knight, PieceType::Bishop}) {
          Move m;
          m.from = from;
          m.to = Square(cto);
          m.promo = pt;
          m.flags |= MoveFlag::Capture | MoveFlag::Promotion;
          if (isEnPass)
            m.flags |= MoveFlag::Enpassent;
          out.push_back(m);
        }
      } else {
        Move m;
        m.from = from;
        m.to = Square(cto);
        m.flags |= MoveFlag::Capture;
        if (isEnPass)
          m.flags |= MoveFlag::Enpassent;
        out.push_back(m);
      }
    }
  }
}

void MoveGenerator::genQueen(const Board &b, Square from,
                             std::vector<Move> &out) {
  const auto &pcs = b.pieces();
  for (int off : QueenOffsets) {
    int to = int(from);
    while (true) {
      int prev = to;
      to += off;
      if (!onBoard(to))
        break;

      if ((off == -1 || off == 1) && std::abs((to & 7) - (prev & 7)) != 1)
        break;
      if ((off == 7 || off == 9 || off == -7 || off == -9) &&
          std::abs((to & 7) - (prev & 7)) != 1)
        break;

      Move m{from, Square(to)};
      if (pcs[to] != PieceType::None) {
        if (b.pieceColor(to) == b.pieceColor(from))
          break;
        m.flags |= MoveFlag::Capture;
        out.push_back(m);
        break;
      }

      out.push_back(m);
    }
  }
}
void MoveGenerator::genKing(const Board &b, Square from,
                            std::vector<Move> &out) {

  const auto &pcs = b.pieces();
  Color side = b.pieceColor(from);
  for (int off : KingOffsets) {
    int to = int(from) + off;

    // file‐wrap check for horizontal/diagonal offsets
    int prevFile = (int(from)) & 7;
    int currFile = to & 7;
    if ((off == +1 || off == -1 || off == +9 || off == -9 || off == +7 ||
         off == -7) &&
        std::abs(currFile - prevFile) != 1)
      continue;

    if (!onBoard(to))
      continue;

    Move m;
    m.from = from;
    m.to = Square(to);
    m.flags = 0; // ensure flags start at zero

    // **1) quiet move to an empty square**
    if (pcs[to] == PieceType::None) {
      out.push_back(m);
      continue;
    }

    // **2) capture an enemy piece**
    if (b.pieceColor(to) != b.pieceColor(from)) {
      m.flags |= MoveFlag::Capture;
      out.push_back(m);
    }
  }

  const uint8_t rights = b.castlingRights();

  if (side == Color::White) {
    // White kingside logic
    if (rights & (1 << 0)) {
      if (pcs[5] == PieceType::None && pcs[6] == PieceType::None) {
        if (!isSquareAttacked(b, Square(4), Color::Black) &&
            !isSquareAttacked(b, Square(5), Color::Black) &&
            !isSquareAttacked(b, Square(6), Color::Black)) {
          Move m;
          m.to = Square(6);
          m.from = Square(4);
          m.flags = MoveFlag::KingSideCastle;
          out.push_back(m);
        }
      }
    }
    // White queenside
    if (rights & (1 << 1)) {
      if (pcs[1] == PieceType::None && pcs[2] == PieceType::None &&
          pcs[3] == PieceType::None) {
        if (!isSquareAttacked(b, Square(4), Color::Black) &&
            !isSquareAttacked(b, Square(3), Color::Black) &&
            !isSquareAttacked(b, Square(2), Color::Black)) {
          Move m;
          m.from = Square(4);
          m.to = Square(2);
          m.flags = MoveFlag::QueenSideCastle;
          out.push_back(m);
        }
      }
    }
  }
  if (side == Color::Black) {
    // Black kingside logic
    if (rights & (1 << 2)) {
      if (pcs[62] == PieceType::None && pcs[61] == PieceType::None) {
        if (!isSquareAttacked(b, Square(62), Color::White) &&
            !isSquareAttacked(b, Square(61), Color::White) &&
            !isSquareAttacked(b, Square(60), Color::White)) {
          Move m;
          m.to = Square(62);
          m.from = Square(60);
          m.flags = MoveFlag::KingSideCastle;
          out.push_back(m);
        }
      }
    }
    // Black queenside
    if (rights & (1 << 3)) {
      if (pcs[57] == PieceType::None && pcs[58] == PieceType::None &&
          pcs[59] == PieceType::None) {
        if (!isSquareAttacked(b, Square(58), Color::White) &&
            !isSquareAttacked(b, Square(60), Color::White) &&
            !isSquareAttacked(b, Square(59), Color::White)) {
          Move m;
          m.from = Square(4);
          m.to = Square(2);
          m.flags = MoveFlag::QueenSideCastle;
          out.push_back(m);
        }
      }
    }
  }
}

std::vector<Move> MoveGenerator::generateLegal(const Board &board) {
  Color us = board.sideToMove();
  Color them = (us == Color::White ? Color::Black : Color::White);

  auto ply = generatePseudoLegal(board);
  std::vector<Move> legal;
  legal.reserve(ply.size());

  for (auto const &m : ply) {
    Board copy = board;
    copy.makeMove(m); // copy.sideToMove() now == them

    Square kingSq = copy.kingSquare(us);
    // test if *them* attack our king
    if (!isSquareAttacked(copy, kingSq, them))
      legal.push_back(m);
  }
  return legal;
}

bool MoveGenerator::isSquareAttacked(const Board &b, Square sq,
                                     Color attacker) {
  const auto &pcs = b.pieces();
  int s = int(sq);

  // 1) Pawn attacks
  if (attacker == Color::White) {
    for (int off : {-9, -7}) {
      int p = s + off;
      if (onBoard(p) && pcs[p] == PieceType::Pawn &&
          b.pieceColor(Square(p)) == Color::White)
        return true;
    }
  } else { // Black
    for (int off : {+7, +9}) {
      int p = s + off;
      if (onBoard(p) && pcs[p] == PieceType::Pawn &&
          b.pieceColor(Square(p)) == Color::Black)
        return true;
    }
  }

  // 2) Knight attacks
  for (int off : knightOffsets) {
    int p = s + off;
    if (!onBoard(p))
      continue;
    if (pcs[p] == PieceType::Knight && b.pieceColor(Square(p)) == attacker)
      return true;
  }

  // 3) Rook & Queen (orthogonal)
  for (int dir : {1, -1, 8, -8}) {
    for (int p = s + dir; onBoard(p); p += dir) {
      if (pcs[p] == PieceType::None)
        continue;
      if ((pcs[p] == PieceType::Rook || pcs[p] == PieceType::Queen) &&
          b.pieceColor(Square(p)) == attacker)
        return true;
      break; // blocked by first piece
    }
  }

  // 4) Bishop & Queen (diagonal)
  for (int dir : {+7, +9, -7, -9}) {
    int prevFile = s & 7; // file of the starting square
    // step p outwards until we fall off the board or wrap
    for (int p = s + dir; onBoard(p) && std::abs((p & 7) - prevFile) ==
                                            1; // ensure no wrap-around
         prevFile = (p & 7), p += dir) {
      if (pcs[p] == PieceType::None)
        continue; // empty square, keep sliding

      // first non-empty square: hit or block
      if ((pcs[p] == PieceType::Bishop || pcs[p] == PieceType::Queen) &&
          b.pieceColor(Square(p)) == attacker)
        return true;
      break;
    }
  }

  // 5) King adjacency
  for (int off : KingOffsets) {
    int p = s + off;
    if (!onBoard(p))
      continue;
    if (pcs[p] == PieceType::King && b.pieceColor(Square(p)) == attacker)
      return true;
  }

  return false;
}
