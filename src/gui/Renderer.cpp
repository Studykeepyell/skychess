#include "../../include/gui/Renderer.hpp"
#include <array>

using namespace chess::gui;
using namespace chess::engine;

Renderer::Renderer(float tileSize) : tile_(tileSize) {
  light_.setSize({tile_, tile_});
  dark_.setSize({tile_, tile_});

  light_.setFillColor(sf::Color(240, 217, 181)); // light squares
  dark_.setFillColor(sf::Color(181, 136, 99));   // dark squares

  if (!font_.loadFromFile("assets/fonts/DejaVuSansCondensed.ttf"))
    throw std::runtime_error("Cannot load font");
}

void Renderer::draw(sf::RenderWindow &win, const Board &board) {
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

  /* ---------- 2. draw piece letters (temp gfx) ---------- */
  for (int idx = 0; idx < 64; ++idx) {
    PieceType p = board.pieces()[idx];
    if (p == PieceType::None)
      continue;

    int f = idx & 7;  // file 0-7
    int r = idx >> 3; // rank 0-7

    sf::Text t;
    t.setFont(font_);
    t.setCharacterSize(static_cast<unsigned>(tile_ * 0.75f));
    t.setFillColor(sf::Color::Black);

    static const std::array<char, 7> glyph = {'K', 'Q', 'R', 'B',
                                              'N', 'P', '?'};
    t.setString(glyph[static_cast<int>(p)]);

    t.setPosition(f * tile_ + tile_ * 0.15f, (7 - r) * tile_ - tile_ * 0.05f);

    win.draw(t);
  }
}
