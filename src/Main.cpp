#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "GoLBoard.h"
#include <iostream>
#include "GoLColor.h"

const std::string version = "1.0";
const auto resX =	1280;
const auto resY =	720;
const auto cellsX = resX/6;
const auto cellsY = resY/6;
const auto text_padding = (resX/cellsX)*4+2;
const sf::Color gridColor(0, 0, 0);

static void updateStatusText(GOLBoard& board, sf::Text& numText);

static void makeGrid(sf::Sprite& grid, sf::RenderTexture& gridTexture);

int main()
{
	sf::RenderWindow window(sf::VideoMode(resX, resY), "Game Of Life v" + version, sf::Style::Close);
	window.setFramerateLimit(120);

	setUpPalette();
	GOLBoard board(cellsX, cellsY, resX, resY, DARK_AND_EARTHY);

	// Text stuff
	// ========================================================================
	sf::Font mc_font;
	if (!mc_font.loadFromFile("resources/fonts/Minecraftia-Regular.ttf"))
	{
		//Couldn't load font
		window.close();
		return -1;
	}
	sf::Text statusText;

	statusText.setFont(mc_font);
	statusText.setCharacterSize(16);
	statusText.setPosition(text_padding, 8 + text_padding);
	statusText.setColor(sf::Color(120, 120, 120));

	// End of Text stuff
	// ========================================================================

	auto clicking = false;
	auto showGrid = true;
	auto showStatus = true;

	// Timing variables
	sf::Clock clock;
	sf::Time elapsed;

	sf::Event event;

	// Make the grid sprite
	sf::Sprite grid;
	sf::RenderTexture gridTexture;
	makeGrid(grid, gridTexture);

	while (window.isOpen())
	{
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
				case sf::Keyboard::G:
					showGrid = !showGrid;			break;
				case sf::Keyboard::D:
					showStatus = !showStatus;		break;
				}
			}
		}

		if (showStatus)
			updateStatusText(board, statusText);

		elapsed = clock.restart();
		board.update(elapsed);

		//window.clear(sf::Color(0,0,0,64));
		window.draw(board);

		if(showGrid)
			window.draw(grid);

		if(showStatus)
			window.draw(statusText);
		window.display();
	}

	return 0;
}

static void updateStatusText(GOLBoard& board, sf::Text& numText)
{
	numText.setString(
		"Alive Cells: " + std::to_string(board.getCellNumber()) +
		"\nSim. Speed:  " + std::to_string(board.getStepSpeed() / 3) +
		(board.isRunning() ? "" : "\n\nPAUSED"));
}

static void makeGrid(sf::Sprite& grid, sf::RenderTexture& gridTexture)
{
	gridTexture.create(resX, resY);

	//Fill not horizontal lines
	for (float x = 0; x < resX; x += float(resX) / cellsX)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(x, 0), gridColor),
			sf::Vertex(sf::Vector2f(x, resY), gridColor)
		};
		gridTexture.draw(line, 2, sf::Lines);
	}

	for (float y = 0; y < resY; y += float(resY) / cellsY)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(0, y), gridColor),
			sf::Vertex(sf::Vector2f(resX, y), gridColor)
		};
		gridTexture.draw(line, 2, sf::Lines);
	}

	grid.setTexture(gridTexture.getTexture());
}

//TODO: (idea) have an additional table and smooth out the values where 
//		there are alive cells over time, creating a blurry smooth effect