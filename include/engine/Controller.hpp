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
  Board board_;
  chess::gui::Renderer render_;

  std::optional<Square> selected_;
  std::vector<Move> legalMoves_;

  std::vector<Move> promoOptions_;
  bool promoPending = false;

  void handleClick(int x, int y);
   Color              promoSide_       = Color::White;
};
} // namespace chess::engine
