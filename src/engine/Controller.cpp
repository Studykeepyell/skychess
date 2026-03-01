#include "engine/Move.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
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
    if (promoPending) {
      render_.drawPromotionPanel(window_, promoSide_, tileSize);
    }
    window_.display();
  }
}

void Controller::handleClick(int x, int y) {
  // 1) Convert screen → board coords
  sf::Vector2f world = window_.mapPixelToCoords({x, y});
  int file = int(world.x / tileSize);
  int rank = 7 - int(world.y / tileSize);

  // 2) If we click off-board, clear everything
  if (file < 0 || file > 7 || rank < 0 || rank > 7) {
    selected_.reset();
    legalMoves_.clear();
    return;
  }

  Square clicked = Square(rank * 8 + file);
  // 3) If we’re in promo-mode, handle panel clicks first
  if (promoPending) {
    float panelSize = tileSize * 2.0f;
    sf::Vector2f panelTL((640.0f - panelSize) * 0.5f,
                         (640.0f - panelSize) * 0.5f);

    PieceType uiPromos[4] = {PieceType::Queen, PieceType::Rook,
                             PieceType::Bishop, PieceType::Knight};

    bool pieceClicked = false;

    // 2) Loop over the 4 promotion options
    for (int i = 0; i < 4; ++i) {
      sf::FloatRect cell{{panelTL.x + float(i % 2) * tileSize,
                          panelTL.y + float(i / 2) * tileSize},
                         {tileSize, tileSize}};

      // FIX: Use 'world' coordinates instead of raw 'x' and 'y' pixels!
      if (cell.contains(world)) {
        pieceClicked = true;
        PieceType chosenPiece = uiPromos[i];
        std::cout << "[DEBUG] Clicked UI piece index: " << i << std::endl;

        bool moveFound = false;
        // Find the exact move in promoOptions_ that matches the chosen piece
        for (auto &m : promoOptions_) {
          if (m.promo == chosenPiece) {
            std::cout << "[DEBUG] Match found! Making promotion move on board."
                      << std::endl;
            board_.makeMove(m);
            selected_ = m.to;

            // Re-compute legal moves for the new piece
            legalMoves_.clear();
            for (auto &new_m : board_.legalMoves()) {
              if (new_m.from == selected_)
                legalMoves_.push_back(new_m);
            }

            moveFound = true;
            break;
          }
        }

        if (!moveFound) {
          std::cout << "[DEBUG] CRITICAL: Piece clicked, but no matching move "
                       "in promoOptions_!"
                    << std::endl;
        }
        break; // Break out of the 4-cell loop
      }
    }

    if (!pieceClicked) {
      std::cout
          << "[DEBUG] Clicked outside the panel bounds. Cancelling promotion."
          << std::endl;
    }

    // 4) Tear down the promotion-chooser state
    promoPending = false;
    promoOptions_.clear();
    return;
  }

  // 4) If a piece is already selected, see if we clicked one of its moves
  if (selected_) {
    for (auto &m : legalMoves_) {
      if (m.from == *selected_ && m.to == clicked) {
        // -- Promotion?  Gather all 4 choices then show panel
        if (m.flags & MoveFlag::Promotion) {
          promoPending = true;
          promoOptions_.clear();
          // Collect *all* promotion variants for this destination
          for (auto &pm : legalMoves_) {
            if (pm.from == *selected_ && pm.to == clicked &&
                (pm.flags & MoveFlag::Promotion)) {
              promoOptions_.push_back(pm);
            }
          }
          // Record which side’s icons to draw
          promoSide_ = board_.pieceColor(*selected_);
          return;
        }

        // -- Normal move
        board_.makeMove(m);
        selected_.reset();
        legalMoves_.clear();
        return;
      }
    }

    // Clicked elsewhere: clear selection
    selected_.reset();
    legalMoves_.clear();
  }

  // 5) No piece selected yet → pick this one and show its moves
  selected_ = clicked;
  legalMoves_.clear();
  for (auto &m : board_.legalMoves()) {
    if (m.from == clicked)
      legalMoves_.push_back(m);
  }
}
