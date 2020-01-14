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
