#pragma once
#include "../engine/Board.hpp"
#include <SFML/Graphics.hpp>

namespace chess::gui {

class Renderer {
public:
  Renderer(float tileSize = 80.0f);
  void draw(sf::RenderWindow &win, const chess::engine::Board &b);

private:
  float tile_;
  sf::RectangleShape light_, dark_;
  sf::Font font_;
  std::map<std::string, sf::Texture> pieceTextures_;
};

} // namespace chess::gui
