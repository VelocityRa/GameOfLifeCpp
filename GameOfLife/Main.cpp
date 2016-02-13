#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "GoLBoard.h"
#include <iostream>
#include "GoLColor.h"

const std::string version = "0.9";
const auto resX =	1280;
const auto resY =	720;
const auto cellsX = resX/5;
const auto cellsY = resY/5;

int main()
{
	sf::RenderWindow window(sf::VideoMode(resX, resY), "Game Of Life v" + version, sf::Style::Close);
	//window.setFramerateLimit(120);
	setUpPalette();
	GOLBoard board(cellsX, cellsY, resX, resY, LIGHT_BLUES);

	auto clicking = false;

	sf::Clock clock;
	sf::Time elapsed;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close(); break;
			case sf::Event::MouseButtonPressed:
				clicking = true;
				board.handleMouse(event.mouseButton.x, event.mouseButton.y);
				break;
			case sf::Event::MouseButtonReleased:
				clicking = false; 
				break;
			case sf::Event::MouseMoved:
				if (clicking)
					board.handleMouse(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::MouseWheelScrolled:
			{
				board.addStepSpeed(event.mouseWheelScroll.delta * 3);
				window.setFramerateLimit(std::max(60, board.getStepSpeed()));
				break;
			}
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Space:
					board.toggleRunningState();
				case sf::Keyboard::C:
					//board.clear
					break;
				}
			}
		}
		elapsed = clock.restart();
		board.update(elapsed);

		//window.clear(sf::Color(0,0,0,64));
		window.draw(board);
		window.display();
	}

	return 0;
}
