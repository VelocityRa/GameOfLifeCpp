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
const auto text_padding = 20;

static void updateCellNumber(GOLBoard& board, sf::Text& numText);

int main()
{
	sf::RenderWindow window(sf::VideoMode(resX, resY), "Game Of Life v" + version, sf::Style::Close);
	window.setFramerateLimit(120);

	setUpPalette();
	GOLBoard board(cellsX, cellsY, resX, resY, DARK_AND_EARTHY);

	auto clicking = false;

	// Text stuff
	sf::Font mc_font;
	if (!mc_font.loadFromFile("resources/fonts/Minecraftia-Regular.ttf"))
	{
		//Couldn't load font
		window.close();
		return -1;
	}

	sf::Text cellNumText;

	cellNumText.setFont(mc_font);
	cellNumText.setCharacterSize(24);
	cellNumText.setPosition(text_padding, 16 + text_padding);
	cellNumText.setColor(sf::Color(100, 100, 100));

	// Timing variables
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
				clicking = false; 	break;
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
					board.toggleRunningState();		break;
				case sf::Keyboard::C:
					board.clearBoard();				break;
				case sf::Keyboard::Z:
					board.cyclePaletteColors(-1);	break;
				case sf::Keyboard::X:
					board.cyclePaletteColors(1);	break;
				}
			}
		}

		updateCellNumber(board, cellNumText);

		elapsed = clock.restart();
		board.update(elapsed);

		//window.clear(sf::Color(0,0,0,64));
		window.draw(board);

		window.draw(cellNumText);
		window.display();
	}

	return 0;
}

static void updateCellNumber(GOLBoard& board, sf::Text& numText)
{
	numText.setString("Cells: " + std::to_string(board.getCellNumber()));
}