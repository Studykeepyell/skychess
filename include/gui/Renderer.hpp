#pragma once
#include <SFML/Graphics.hpp>
#include "../engine/Board.hpp"

namespace chess::gui {

class Renderer {
public:
    Renderer(float tileSize = 80.0f);
    void draw(sf::RenderWindow& win,const chess::engine::Board& b);


private:
    float tile_;
    sf::RectangleShape light_, dark_;
    sf::Font font_;
};

} //namespace chess::gui
