#include "GoLColor.h"

#define AddCols(c1r,c1g,c1b,c2r,c2g,c2b) \
palette.push_back({sf::Color(c1r,c1g,c1b),sf::Color(c2r,c2g,c2b)})

std::vector<colors> palette;

// Push all the color pairs to our palette vector
// TODO: Possibly use std::map 
void setUpPalette()
{
	AddCols(255,255,255,	// BLACK_AND_WHITE
			0,	0,	0);
	AddCols(123, 22, 3,		// RED_AND_PURPLE
			43,	20,	90);
	AddCols(167, 219, 216,	// LIGHT_BLUES
			95, 200, 221);
	AddCols(170, 70, 40,	// DARK_AND_EARTHY
			68, 32, 26);
}
#undef Col(x)
#undef AddCols(x,y)