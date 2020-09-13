#include <iostream>

#include "Doryen/Image/Mipmap.hpp"

unsigned int Doryen::Mipmap::getLevels(
		unsigned int width,
		unsigned int height)
{
	// Original size is: 30 x 40
	// 	Resize to: 15 x 20
	// 	Resize to: 7 x 10
	// 	Resize to: 3 x 5
	// 	Resize to: 1 x 2
	// 	Resize to: 0 x 1
	// End resize, total levels is: 5

	unsigned int curw = width;
	unsigned int curh = height;

	std::cout << "Original size is: " << width << " x " << height << '\n';

	int levels = 0;

	while (curw > 0 && curh > 0)
	{
		levels++;
		curw >>= 1;
		curh >>= 1;

		std::cout << "Resize to: " << curw << " x " << curh << '\n';

	}

	std::cout << "End resize, total levels is: " << levels << "\n\n";

	return levels;
}

Doryen::Color Doryen::Mipmap::getColorAt(int _x, int _y) const
{
	return (*this)[_x + _y * width];
}

void Doryen::Mipmap::setColorAt(int _x, int _y, const Doryen::Color& _color)
{
	(*this)[_x + _y * width] = _color;
}

void Doryen::Mipmap::setDirty(bool _dirty)
{
	dirty = _dirty;
}
