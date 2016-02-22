#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/String.hpp>

enum colorName;

class GOLBoard :
	public sf::Drawable
{
public:
	enum class CellType
	{
		Dead = 0,
		Alive,
		MarkedDeath,
		MarkedBirth
	};

	// Constructor/Destructor
	GOLBoard::GOLBoard(const uint32_t cellsX, const uint32_t cellsY, 
					const uint32_t resX, const uint32_t resY, colorName col);
	~GOLBoard();

	// Helper methods (getters/setters/etc)
	// ========================================================================
	CellType getCell(uint32_t i, uint32_t j)		const
	{
		return cells[i][j];
	}
	void	setCell(uint32_t i, uint32_t j, CellType cell)
	{
		//printf("i: %u j: %u\n", i, j);
		cells[i][j] = cell;
	}
	bool	isAlive(uint32_t i, uint32_t j)			const
	{
		auto cell = getCell(i, j);
		return cell == CellType::Alive
			|| cell == CellType::MarkedDeath;
	}
	bool	isDead(uint32_t i, uint32_t j)			const
	{
		auto cell = getCell(i, j);
		return cell == CellType::Dead
			|| cell == CellType::MarkedBirth; //commenting this creates another
	}										  //cool cellular automaton!
	bool	isMarkedDeath(uint32_t i, uint32_t j)	const
	{
		return getCell(i, j) == CellType::MarkedDeath;
	}
	bool	isMarkedBirth(uint32_t i, uint32_t j)	const
	{
		return getCell(i, j) == CellType::MarkedBirth;
	}
	void	birthCell(uint32_t i, uint32_t j)	
	{
		setCell(i, j, CellType::Alive);
	}
	void	killCell(uint32_t i, uint32_t j)
	{
		setCell(i, j, CellType::Dead);
	}
	void	markDeathCell(uint32_t i, uint32_t j)
	{
		setCell(i, j, CellType::MarkedDeath);
	}	
	void	markBirthCell(uint32_t i, uint32_t j)
	{
		setCell(i, j, CellType::MarkedBirth);
	}

	bool	isRunning() const
	{
		return running;
	}
	void	toggleRunningState()
	{
		running = !running;
	}
	void	addStepSpeed(int speedDelta)
	{
		stepSpeed = std::max(0, stepSpeed + speedDelta);
	}
	int		getStepSpeed() const 
	{
		return stepSpeed;
	}
	unsigned int getCellNumber() const
	{
		return cellNum;
	}
	colorName	getCurColor() const
	{
		return currColor;
	}
	void clearBoard()
	{
		for (auto x = 0; x < cellsX; x++)
			for (auto y = 0; y < cellsY; y++)
				cells[x][y] = CellType::Dead;
		cellNum = 0;
	}
	// ========================================================================
	// End of helper methods

	void GOLBoard::update(sf::Time& elapsedTime);
	void updateTexture();
	void updateSprite();
	void handleMarked();
	void stepSimulation();
	void populateRandom(uint32_t nCells);
	void changeColors(colorName col);
	void GOLBoard::cyclePaletteColors(int dir);
	uint8_t	getNeighbours(uint32_t i, uint32_t j) const;
	void handleMouse(int x, int y);

private:
	virtual void GOLBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(sprite);
	}

	int stepSpeed = 15;	// Simulation speed in steps per second
	const uint32_t cellsX, cellsY, resX, resY;
	const float sizeX, sizeY;
	bool running, changed;

	// make a C array instead of a vector for performance
	CellType **cells;
	sf::RectangleShape rect;
	sf::RenderTexture boardTexture;
	sf::Texture image;
	sf::Sprite sprite;
	sf::Time elapsedSum;
	colorName currColor;
	int cellNum;
};
