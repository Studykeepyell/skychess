#include "../../include/gui/Renderer.hpp"
#include <array>
#include <stdexcept>

using namespace chess::gui;
using namespace chess::engine;

Renderer::Renderer(float tileSize) : tile_(tileSize) {
  light_.setSize({tile_, tile_});
  dark_.setSize({tile_, tile_});

  light_.setFillColor(sf::Color(240, 217, 181)); // light squares
  dark_.setFillColor(sf::Color(181, 136, 99));   // dark squares

  std::vector<std::string> pieceKeys = {"WP", "WB", "WN", "WR", "WQ", "WK",
                                        "BP", "BB", "BN", "BR", "BQ", "BK"};

  for (const auto &key : pieceKeys) {
    sf::Texture texture;
    if (!texture.loadFromFile("assets/pieces/chess_pieces/" + key + ".png"))
      throw std::runtime_error("can not load the texture" + key);
    pieceTextures_[key] = texture;
  }
  if (!font_.loadFromFile("assets/fonts/DejaVuSansCondensed.ttf"))
    throw std::runtime_error("Cannot load font");
  highlightSquare_.setSize({tile_, tile_});
  highlightSquare_.setFillColor({sf::Color(0, 255, 0, 100)});
}

void Renderer::draw(sf::RenderWindow &win, const Board &board,
                    const std::vector<chess::engine::Move> &highlightedMoves,
                    std::optional<chess::engine::Square> selected) {
  /* ---------- 1. draw the checkerboard ---------- */
  for (int r = 0; r < 8; ++r) {
    for (int f = 0; f < 8; ++f) {
      bool isLight = ((r + f) % 2 == 0);
      sf::RectangleShape &sq = isLight ? light_ : dark_;

      // place (0,0) at bottom-left like a real board
      sq.setPosition(f * tile_, (7 - r) * tile_);
      win.draw(sq);
    }
  }

  // highlight legal moves
  for (const auto &moves : highlightedMoves) {
    int f = moves.to % 8;
    int r = moves.to / 8;
    highlightSquare_.setPosition(f * tile_, (7 - r) * tile_);
    win.draw(highlightSquare_);
  }

  for (int i = 0; i < 64; i++) {
    PieceType p = board.pieces()[i];
    Color c = board.pieceColor(static_cast<Square>(i));

    if (p == PieceType::None)
      continue;

    int f = i % 8;
    int r = i / 8;
    std::string key = (c == Color ::White ? "W" : "B");
    switch (p) {
    case PieceType::Bishop:
      key += "B";
      break;
    case PieceType::Knight:
      key += "N";
      break;
    case PieceType::King:
      key += "K";
      break;
    case PieceType::Queen:
      key += "Q";
      break;
    case PieceType::Rook:
      key += "R";
      break;
    case PieceType::Pawn:
      key += "P";
      break;
    default:
      continue;
    }
    sf::Sprite sprite;
    sprite.setTexture(pieceTextures_.at(key));

    sprite.setScale(tile_ / sprite.getTexture()->getSize().x,
                    tile_ / sprite.getTexture()->getSize().y);
    sprite.setPosition(f * tile_, (7 - r) * tile_);
    win.draw(sprite);
  }
}
