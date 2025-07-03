#include "../../include/engine/Controller.hpp"

using namespace chess::engine;

Controller::Controller()
    : window_(sf::VideoMode(sf::Vector2u(640, 640)), "chess GUI"),
      render_(tileSize) {}

void Controller::run() {
  while (window_.isOpen()) {
    // SFML 3.x: pollEvent() returns std::optional<sf::Event>
    while (auto optEvent = window_.pollEvent()) {
      auto &event = *optEvent; // unwrap the optional

      if (event.type == sf::Event::Closed) {
        window_.close();
      } else if (event.type == sf::Event::MouseButtonPressed &&
                 event.mouseButton.button == sf::Mouse::Button::Left) {
        handleClick(event.mouseButton.x, event.mouseButton.y);
      }
    }

    window_.clear();
    render_.draw(window_, board_, legalMoves_, selected_);
    window_.display();
  }
}
