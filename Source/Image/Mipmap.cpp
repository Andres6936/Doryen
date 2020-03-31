#include "Image/Mipmap.hpp"

unsigned int Doryen::Mipmap::getLevels(
		unsigned int width,
		unsigned int height)
{
	unsigned int curw = width;
	unsigned int curh = height;

	int levels = 0;

	while (curw > 0 && curh > 0)
	{
		levels++;
		curw >>= 1;
		curh >>= 1;
	}

	return levels;
}

Doryen::Color Doryen::Mipmap::getColorAt(int _x, int _y) const
{
	return buf[_x + _y * width];
}
