// SkyChess main  —  SFML 2.x version
#include <SFML/Graphics.hpp>

#include "engine/Board.hpp"
#include "gui/Renderer.hpp"

int main()
{
    // Classic 2-argument VideoMode constructor (width, height)
    sf::RenderWindow window(sf::VideoMode(640, 640),
                            "Chess – start-up");
    window.setFramerateLimit(60);

    chess::engine::Board  board;
    chess::gui::Renderer  renderer(80.f);

    while (window.isOpen())
    {
        sf::Event ev;                            // <- event object
        while (window.pollEvent(ev))             // <- pass by ref
        {
            if (ev.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        renderer.draw(window, board);
        window.display();
    }
    return 0;
}

