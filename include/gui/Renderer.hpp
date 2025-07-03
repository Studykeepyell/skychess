#pragma once
#include "../engine/Board.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>
#include <string>
namespace chess::gui {

class Renderer {
public:
  Renderer(float tileSize = 80.0f);
  void draw(sf::RenderWindow &win, const chess::engine::Board &b,
            const std::vector<chess::engine::Move> &highlightedMoves = {},
            std::optional<chess::engine::Square> selected = std::nullopt);

private:
  float tile_;
  sf::RectangleShape light_, dark_;
  sf::Font font_;
  std::map<std::string, sf::Texture> pieceTextures_;

  sf::RectangleShape highlightSquare_;
};

} // namespace chess::gui
