// in src/engine/Board.cpp
#include "engine/Board.hpp"
#include "engine/MoveGenerator.hpp" // now this is the correct path
#include <array>                    // for your helpers
#include <cassert>
using namespace chess::engine;

// helper to put R–N–B–Q–K–B–N–R on one rank:
static void placeBackRank(std::array<PieceType, 64> &sq,
                          std::array<Color, 64> &col, int rank, Color side) {
  constexpr std::array<PieceType, 8> order{
      PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
      PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook};
  int base = rank * 8;
  for (int file = 0; file < 8; ++file) {
    sq[base + file] = order[file];
    col[base + file] = side;
  }
}

// helper to put eight pawns on one rank:
static void placePawnRank(std::array<PieceType, 64> &sq,
                          std::array<Color, 64> &col, int rank, Color side) {
  int base = rank * 8;
  for (int i = 0; i < 8; ++i) {
    sq[base + i] = PieceType::Pawn;
    col[base + i] = side;
  }
}

Board::Board() : epSquare_(64), stm_(Color::White) { setStartPosition(); }

Board::Board(const std::string & /*fen*/)
    : Board() // default to standard setup; you can parse FEN here later
{}

void Board::setStartPosition() {
  // clear out any old data
  squares_.fill(PieceType::None);
  colours_.fill(Color::None);
  epSquare_ = Square(64);
  stm_ = Color::White;

  // white back rank & pawns
  placeBackRank(squares_, colours_, 0, Color::White);
  placePawnRank(squares_, colours_, 1, Color::White);

  // black pawns & back rank
  placePawnRank(squares_, colours_, 6, Color::Black);
  placeBackRank(squares_, colours_, 7, Color::Black);

  castleRights_ = 0b1111;

  // record king starting squares for fast access
  kinSq_[static_cast<int>(Color::White)] = Square(4);
  kinSq_[static_cast<int>(Color::Black)] = Square(60);
}

std::vector<Move> Board::legalMoves() const {
  return MoveGenerator::generateLegal(*this);
}

void Board::revokeKingRights(Color side) {
  if (side == Color::White)
    castleRights_ &= ~(0b0011);
  else
    castleRights_ &= ~(0b1100);
}

void Board::revokeRookRights(Color side, bool kingside) {
  int bit = (side == Color::White) ? (kingside ? 0 : 1) : (kingside ? 2 : 3);
  castleRights_ &= ~(1 << bit);
}

void Board::revokeRightsOnRookCapture(Square to) {
  switch (to) {
  case Square(0):
    revokeRookRights(Color::White, false);
    break;
  case Square(7):
    revokeRookRights(Color::White, true);
    break;

  case Square(56):
    revokeRookRights(Color::Black, false);
    break;

  case Square(63):
    revokeRookRights(Color::Black, true);
    break;
  default:
    break;
  }
}

void Board::updateCastleRights(const Move &m) {
  Color Mover = pieceColor(m.from);

  if (squares_[m.from] == PieceType::King) {
    revokeKingRights(Mover);
    kinSq_[static_cast<int>(Mover)] = m.to;
  }

  if (squares_[m.from] == PieceType::Rook) {
    bool isKS = (m.from == Square(7) && Mover == Color::White) ||
                (m.from == Square(63) && Mover == Color::Black);

    revokeRookRights(Mover, isKS);
  }

  if (squares_[m.to] == PieceType::Rook) {
    revokeRightsOnRookCapture(m.to);
  }
}

void Board::applyCastleRookMove(const Move &m) {
  // White O-O: king e1→g1, rook h1→f1
  if (m.from == Square(4) && m.to == Square(6)) {
    squares_[5] = squares_[7];
    colours_[5] = colours_[7];
    squares_[7] = PieceType::None;
    colours_[7] = Color::None;
  }
  // White O-O-O: king e1→c1, rook a1→d1
  else if (m.from == Square(4) && m.to == Square(2)) {
    squares_[3] = squares_[0];
    colours_[3] = colours_[0];
    squares_[0] = PieceType::None;
    colours_[0] = Color::None;
  }
  // Black O-O: king e8→g8, rook h8→f8
  else if (m.from == Square(60) && m.to == Square(62)) {
    squares_[61] = squares_[63];
    colours_[61] = colours_[63];
    squares_[63] = PieceType::None;
    colours_[63] = Color::None;
  }
  // Black O-O-O: king e8→c8, rook a8→d8
  else if (m.from == Square(60) && m.to == Square(58)) {
    squares_[59] = squares_[56];
    colours_[59] = colours_[56];
    squares_[56] = PieceType::None;
    colours_[56] = Color::None;
  }
}

bool Board::makeMove(const Move &m) {
  int from = int(m.from);
  int to = int(m.to);
  assert(from >= 0 && from < 64);
  assert(to >= 0 && to < 64);
  epSquare_ = Square(64);

  updateCastleRights(m);
  if (m.flags & MoveFlag::KingSideCastle)
    applyCastleRookMove(m);
  if (m.flags & MoveFlag::QueenSideCastle)
    applyCastleRookMove(m);

  if (m.flags & MoveFlag::Enpassent) {
    bool isWhite = (stm_ == Color::White);
    int capIdx = int(m.to) + (isWhite ? -8 : +8);
    squares_[capIdx] = PieceType::None;
    colours_[capIdx] = Color::None;
  }

  squares_[m.to] = squares_[m.from];
  colours_[m.to] = colours_[m.from];
  squares_[m.from] = PieceType::None;

  colours_[m.from] = Color::None;

  if (squares_[m.to] == PieceType::Pawn) {
    int delta = int(m.to) - int(m.from);
    if (delta == +16) {

      epSquare_ = Square(m.from + 8);
    } else if (delta == -16) {

      epSquare_ = Square(m.from - 8);
    }
  }

  stm_ = (stm_ == Color::White) ? Color::Black : Color::White;
  return true;
}
