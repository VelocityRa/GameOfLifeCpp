#include "GoLColor.h"

#define AddCols(c1r,c1g,c1b,c2r,c2g,c2b) \
palette.push_back({sf::Color(c1r,c1g,c1b),sf::Color(c2r,c2g,c2b)})

std::vector<colors> palette;
void setUpPalette()
{
	AddCols(255,255,255,
			0,	0,	0);
	AddCols(123, 22, 3,
			43,	20,	90);
	AddCols(167, 219, 216,
		95, 200, 221);
}
#undef Col(x)
#undef AddCols(x,y)