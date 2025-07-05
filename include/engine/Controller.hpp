#pragma once
#include "../engine/Board.hpp"
#include "../gui/Renderer.hpp"
#include <SFML/Graphics.hpp> // brings in sf::Window, sf::RenderWindow, sf::Event, pollEvent(), etc.
//

namespace chess::engine {
class Controller {

public:
  Controller();

  void run();

private:
  float tileSize = 80.0f;

  sf::RenderWindow window_;
  chess::engine::Board board_;
  chess::gui::Renderer render_;

  std::optional<chess::engine::Square> selected_;
  std::vector<chess::engine::Move> legalMoves_;

  void handleClick(int x, int y);
};
} // namespace chess::engine
