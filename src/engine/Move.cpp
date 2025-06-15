#include "../../include/engine/Move.hpp"
#include <cassert>
#include <string>
using namespace chess::engine;

/* String to algebraic helpers */

static char fileToChar(int f) { return char('a' + f); }
static char rankToChar(int r) { return char('1' + r); }
static Square strToSquare(const std::string &s) {
  return Square((s[0] - 'a') + 8 * (s[1] - '1'));
}

/*public helpers*/

std::string Move::toUci() const {
  std::string s;
  s += fileToChar(from & 7);
  s += rankToChar(from >> 3);
  s += fileToChar(to & 7);
  s += rankToChar(to >> 3);

  if (promo != PieceType::None) {
    switch (promo) {
    case PieceType::Knight:
      s += 'n';
      break;
    case PieceType::Bishop:
      s += 'b';
      break;
    case PieceType::Rook:
      s += 'r';
      break;
    case PieceType::Queen:
      s += 'q';
      break;
    default:
      assert(false && "Invalid promotion piece");
    }
  }
  return s;
}

Move Move::fromUci(const std::string &str) {
  assert(str.size() == 4 || str.size() == 5);
  Move m;
  m.from = strToSquare(str.substr(0, 2));
  m.to = strToSquare(str.substr(2, 2));
  if (str.size() == 5) {
    char c = str[4];
    m.promo = (c == 'q')   ? PieceType::Queen
              : (c == 'r') ? PieceType::Rook
              : (c == 'b') ? PieceType::Bishop
              : (c == 'n') ? PieceType::Knight
                           : PieceType::None;
    m.flags |= MoveFlag::Promotion;
  }
  return m;
}
