#include "gui/Renderer.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include <vector>

using namespace chess::gui;
using namespace chess::engine;

Renderer::Renderer(float tileSize) : tile_(tileSize) {
  // Prepare board-square shapes
  light_.setSize({tile_, tile_});
  dark_.setSize({tile_, tile_});
  light_.setFillColor(sf::Color(240, 217, 181));
  dark_.setFillColor(sf::Color(181, 136, 99));

  // Load piece textures
  std::vector<std::string> pieceKeys = {"WP", "WB", "WN", "WR", "WQ", "WK",
                                        "BP", "BB", "BN", "BR", "BQ", "BK"};
  for (auto const &key : pieceKeys) {
    sf::Texture tex;
    if (!tex.loadFromFile("assets/pieces/chess_pieces/" + key + ".png"))
      throw std::runtime_error("Cannot load texture: " + key);
    pieceTextures_[key] = std::move(tex);
  }

  // Load font (SFML3: openFromFile)
  if (!font_.openFromFile("assets/fonts/DejaVuSansCondensed.ttf"))
    throw std::runtime_error("Cannot load font");

  // Highlighted-move square setup
  highlightSquare_.setSize({tile_, tile_});
  highlightSquare_.setFillColor({0, 255, 0, 100});
}

void Renderer::draw(sf::RenderWindow &win, const Board &board,
                    const std::vector<Move> &highlightedMoves,
                    std::optional<Square> /*selected*/
) {
  // 1) Draw chessboard grid
  for (int r = 0; r < 8; ++r) {
    for (int f = 0; f < 8; ++f) {
      bool isLight = ((r + f) % 2 == 0);
      sf::RectangleShape &sq = isLight ? light_ : dark_;
      sq.setPosition({float(f) * tile_, float(7 - r) * tile_});
      win.draw(sq);
    }
  }

  // 2) Highlight legal moves
  for (auto const &m : highlightedMoves) {
    float fx = float(m.to % 8) * tile_;
    float fy = float(7 - (m.to / 8)) * tile_;
    highlightSquare_.setPosition({fx, fy});
    win.draw(highlightSquare_);
  }

  // 3) Draw pieces
  for (int i = 0; i < 64; ++i) {
    PieceType p = board.pieces()[i];
    if (p == PieceType::None)
      continue;

    Color c = board.pieceColor(static_cast<Square>(i));
    int f = i % 8;
    int r = i / 8;

    std::string key = (c == Color::White ? "W" : "B");
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

    auto const &tex = pieceTextures_.at(key);
    sf::Sprite sprite(tex);
    auto sz = tex.getSize();
    // SFML3: setScale takes a Vector2f
    sprite.setScale({tile_ / float(sz.x), tile_ / float(sz.y)});
    sprite.setPosition({float(f) * tile_, float(7 - r) * tile_});
    win.draw(sprite);
  }
}
