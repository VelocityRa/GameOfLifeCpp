#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Time.hpp>
#include "GOLBoard.h"
#include "GoLColor.h"
#include <cassert>


GOLBoard::GOLBoard(const uint32_t cellsX, const uint32_t cellsY,
	const uint32_t resX, const uint32_t resY, const colorName col) :
	cellsX(cellsX),
	cellsY(cellsY),
	resX(resX),
	resY(resY),
	sizeX(float(resX) / cellsX),
	sizeY(float(resY) / cellsY),
	currColor(col),
	running(true), // flag to keep track of when we need to update the Board texture
	changed(true)  // Are we running the simulation?
{
	// Make a 2 dimensional dynamic array of CellTypes to hold our cells
	cells = static_cast<CellType**>(calloc(cellsX, sizeof(CellType*)));
	for (size_t x = 0; x < cellsX; x++)
	{
		cells[x] = static_cast<CellType*>(calloc(cellsY, sizeof(CellType)));
	}

	if (!boardTexture.create(resX, resY))
	{
		printf("error creating texture");
		exit(-1);
	}
	//srand(time(nullptr));
	rect.setSize(sf::Vector2f(sizeX, sizeY));
	rect.setFillColor(palette[currColor].fg);

	//sprite.setColor(sf::Color(255, 255, 255, 170));

	int initCellNum = cellsX*cellsY / 8; // Fill up an eighth of the cells
	populateRandom(initCellNum);	
}

GOLBoard::~GOLBoard()
{
	for (size_t x = 0; x < cellsX; x++)
	{
		free(cells[x]);
	}
	free(cells);
}

void GOLBoard::updateTexture()
{
	boardTexture.clear(palette[currColor].bg);
	// TODO: Optimize with a queue, so that we dont need to check every tile every frame
	for (auto x = 0; x < cellsX; x++)
	{
		for (auto y = 0; y < cellsY; y++)
		{
			if (isDead(x, y))
			{
				continue;
			}
			/*
			if (isMarkedBirth(x, y))
			{
				rect.setFillColor(sf::Color::Cyan);
			} else if (isMarkedDeath(x,y))  {
				rect.setFillColor(sf::Color::Yellow);
			} else if (isAlive(x, y)) {
				rect.setFillColor(sf::Color::White);
			} else {
				continue;
			}
			*/
			rect.setPosition(sf::Vector2f(x * sizeX, y * sizeY));	//set the appropriate position to the Rect
			//sf::RenderStates states(sf::BlendAdd);

			boardTexture.draw(rect);	// Draw it on our board texture
				
			//rect.setFillColor(sf::Color::White);
		}
	}
	changed = false;
}

void GOLBoard::updateSprite()
{
	sprite.setTexture(boardTexture.getTexture());
}

void GOLBoard::handleMarked()
{
	for (auto x = 0; x < cellsX; x++)
		for (auto y = 0; y < cellsY; y++)
			if (isMarkedDeath(x, y))
				killCell(x, y);
			else if (isMarkedBirth(x, y))
				birthCell(x, y);
}

void GOLBoard::stepSimulation()
{
	cellNum = 0;
	for (auto x = 0; x < cellsX; x++)
		for (auto y = 0; y < cellsY; y++)
		{
			if (isAlive(x, y)) cellNum++;
			if (isDead(x, y))
			{
				if (getNeighbours(x, y) == 3) // If a dead cell has 3 neighboors, give it life!
				{
					markBirthCell(x, y);
				}
			}
			else {
				//printf("x: %d y: %d n: %d\n", x, y, getNeighbours(x, y));
				switch (getNeighbours(x, y))
				{
				case 2: // A cell remains alive only if it has 2 or 3 neighboors
				case 3:
					break;
				default:
					markDeathCell(x, y); // In every other case, it gets marked to die on the next tick
				}
			}
		}
	handleMarked(); // Handle MarkedBirth and MarkedDie cells
	changed = true;
}

void GOLBoard::update(sf::Time& _elapsed)
{
	if(changed)
	{
		updateTexture();
		updateSprite();
		//handleMarked();	Call that in stepSimulation for now
	}
	elapsedSum += _elapsed;
	if(running & elapsedSum.asSeconds() > 1.f/stepSpeed)
	{
		stepSimulation();
		//std::cout << cellNum << std::endl;
		elapsedSum = sf::seconds(0.f);
	}
}

uint8_t GOLBoard::getNeighbours(uint32_t i, uint32_t j) const
{
	auto nb = 0;
	if(i==0 || j==0 || i==cellsX-1 || j==cellsY-1)  // Are we on an edge?
	{
		//printf("EDGE x: %d y: %d\n", i, j);
		for (auto x = -1; x <= 1; x++)
			for (auto y = -1; y <= 1; y++)
				if (!(x + i == -1 || y + j == -1 || x + i == cellsX || y + j == cellsY)) // Are we inside the board?
					if (isAlive(x + i, y + j))
						nb++;
	} else { // We aren't on an edge so just check all 9 surrounding cells without bounds checking
		for (auto x = -1; x <= 1; x++)
			for (auto y = -1; y <= 1; y++)
				if (isAlive(x + i, y + j))
					nb++;
	}
	if(isAlive(i, j))
		return std::max(0, nb - 1);
	return nb;

}

void GOLBoard::handleMouse(int x, int y)
{
	if (x > 0 && y > 0 && x < resX && y < resY)  // Are we on an edge?
	{
		setCell(x / (float(resX) / cellsX), -y / (float(resY) / cellsY) + cellsY,
			sf::Mouse::isButtonPressed(sf::Mouse::Left) ? CellType::Alive : CellType::Dead);
		changed = true;
	}
}

// Assumes that input is not more than the board can fit
void GOLBoard::populateRandom(uint32_t nCells)
{
	cellNum += nCells;
	while (nCells)
	{
		auto xRand = rand() % cellsX;
		auto yRand = rand() % cellsY;
		if (isDead(xRand, yRand))
		{
			setCell(xRand, yRand, CellType::Alive);
			nCells--;
		}
	}
}

void GOLBoard::changeColors(colorName col)
{
	currColor = col;
	// We need to change that ourselves here since changes to the fg color are not checked when drawing
	rect.setFillColor(palette[currColor].fg);	
}

// dir is 1 going forwards, -1 if going backwards
void GOLBoard::cyclePaletteColors(int dir)
{
	assert(dir != 1 || dir != -1);

	auto newCol = static_cast<colorName>(std::max(0, getCurColor() + dir) % NUM_COLORS);
	changeColors(newCol);
	changed = true;		// Change texture immediately (at next update())
}