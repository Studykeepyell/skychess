#include "../../include/gui/Renderer.hpp"
#include <array>

using namespace chess::gui;
using namespace chess::engine;

Renderer::Renderer(float tileSize) : tile_(tileSize) {
	light_.setSize({tile_, tile_});
	dark_.setSize({tile_,tile_});
	light_.setFillColor(sf::Color(240,217,181));
	dark_.setFillColor(sf::Color(181,136,99));

	font_.LoadFromFile("../../assets/fonts/DejaVuSansCondensed.ttf");
}

void Renderer::draw(sf::RenderWindow& win, const Board& board) {
    // … code for drawing squares …

    // Draw piece‐letters on top:
    for (int idx = 0; idx < 64; ++idx) {
        PieceType p = board.pieces()[idx];
        if (p == PieceType::None)
            continue;

        int file = idx & 7;        // 0..7
        int rank = idx >> 3;       // 0..7

        // Construct an sf::Text (empty string for now):
        sf::Text t{};                     
        t.setFont(font_);                
        t.setCharacterSize(static_cast<unsigned>(tile_ * 0.75f));
        t.setFillColor(sf::Color::Black);

        // Correctly set the letter using setString (capital S):
        static const std::array<char, 7> glyphs = {'K','Q','R','B','N','P','?'};
        char letter = glyphs[ static_cast<int>(p) ];  
        t.setString(std::string(1, letter));          

        // Correct setPosition call (all variables must exist exactly as named):
        float xpos = file * tile_ + tile_ * 0.15f;
        float ypos = (7 - rank) * tile_ - tile_ * 0.05f;
        t.setPosition(xpos, ypos);

        win.draw(t);
    }
}
