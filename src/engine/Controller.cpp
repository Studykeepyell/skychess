#include "../../include/engine/Controller.hpp"
#include <string>
using namespace chess::engine;

Controller::Controller()
    : window_(sf::VideoMode(sf::Vector2u(640, 640)), "chess GUI"),
      render_(tileSize) {}

void Controller::run() {
  while (window_.isOpen()) {
    // In SFML 3, pollEvent returns a std::optional<sf::Event>
    while (auto optEvent = window_.pollEvent()) {
      const sf::Event &event = *optEvent; // unwrap the optional

      if (event.is<sf::Event::Closed>()) {
        window_.close();
      } else if (const auto *mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left)
          handleClick(mouse->position.x, mouse->position.y);
      }
    }
    window_.clear();
    render_.draw(window_, board_, legalMoves_, selected_);
    window_.display();
  }
}

void Controller::handleClick(int x, int y) {
  int file = x / static_cast<int>(tileSize);
  int rank = 7 - y / static_cast<int>(tileSize);
  if (file < 0 || file >= 8 || rank < 0 || rank >= 8)
    return;

  selected_ = Square(rank * 8 + file);
  legalMoves_ = board_.legalMoves();
}
