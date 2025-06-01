//Skychess main
#include <SFML/Graphics.hpp>
#include "../include/engine/Board.hpp"
#include "../include/gui/Renderer.hpp"


int main(){

	sf::RenderWindow window(
		sf::VideoMode(640,640),
		 "Chess - start-up");
	window.setFramerateLimit(60);

	chess::engine::Board board;
	chess::gui::Renderer renderer(80.0f);

	while (window.isOpen()){
		sf::Event ev;
		while(window.pollEvent(ev)){
			if (ev.type == sf::Event::Closed) window.close();
			// TODO: drag_drop -> board.makeMove(...)
		}

		window.clear();
		renderer.draw(window,board);
		window.display();
	}
	return 0;

}
