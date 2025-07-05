// in src/engine/Board.cpp
#include "engine/Board.hpp"
#include "engine/MoveGenerator.hpp" // now this is the correct path
#include <array>                    // for your helpers
#include <stdexcept>

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
}

std::vector<Move> Board::legalMoves() const {
  return MoveGenerator::generateLegal(*this);
}

bool Board::makeMove(const Move &m) {

  epSquare_ = 64;

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
