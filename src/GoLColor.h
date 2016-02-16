#pragma once
#include <SFML/Graphics/Color.hpp>
#include <vector>

struct colors
{
	sf::Color fg;
	sf::Color bg;
};

extern std::vector<colors> palette;
void setUpPalette();

enum colorName
{
	BLACK_AND_WHITE,
	RED_AND_PURPLE,
	LIGHT_BLUES,
	DARK_AND_EARTHY
};