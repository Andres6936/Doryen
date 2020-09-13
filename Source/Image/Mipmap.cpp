#include "Doryen/Image/Mipmap.hpp"

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
