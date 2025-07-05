#include <engine/Controller.hpp>
#include <iostream>
#include <ostream>
#include <string>
using namespace chess::engine;

Controller::Controller()

    : window_(sf::VideoMode(sf::Vector2u(640, 640)), "chess GUI"),
      render_(tileSize) {
  board_.setStartPosition();
}

void Controller::run() {
  while (window_.isOpen()) {
    // In SFML 3, pollEvent returns a std::optional<sf::Event>
    while (auto optEvent = window_.pollEvent()) {
      const sf::Event &event = *optEvent; // unwrap the optional

      if (event.is<sf::Event::Closed>()) {
        window_.close();
      } else if (const auto *mouse =
                     event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left)
          handleClick(mouse->position.x, mouse->position.y);
        std::cout << "beep" << std::endl;
      }
    }
    window_.clear();
    render_.draw(window_, board_, legalMoves_, selected_);

    window_.display();
  }
}

void Controller::handleClick(int x, int y) {
  // convert raw window pixel croods into worlds croods

  sf::Vector2f worldPos = window_.mapPixelToCoords({x, y});

  // 2) Divide by your tile size to get file & rank in [0..8)
  int file = static_cast<int>(worldPos.x / tileSize);
  int rank = 7 - static_cast<int>(worldPos.y / tileSize);

  if (file < 0 || file >= 8 || rank < 0 || rank >= 8) {
    selected_.reset();
    legalMoves_.clear();
    return;
  }
  Square clicked = Square(rank * 8 + file);

  if (selected_) {
    for (auto &m : legalMoves_) {
      if (m.from == *selected_ && m.to == clicked) {
        board_.makeMove(m);
        selected_.reset();
        legalMoves_.clear();
        return;
      }
    }
  }

  selected_ = clicked;
  // Get the full list, then keep only those whose .from matches
  auto all = board_.legalMoves();
  legalMoves_.clear();
  for (auto &m : all) {
    if (m.from == selected_)
      legalMoves_.push_back(m);
  }
}
