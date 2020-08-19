#include "LineListener.hpp"

bool RenderLine::drawPoint(int x, int y)
{
	if (x >= 0 && y >= 0 && x < SAMPLE_SCREEN_WIDTH && y < SAMPLE_SCREEN_HEIGHT)
	{
		console.setCellBackground(x, y, Doryen::Palette::PRIMARY_LIGHT, backFlag);
	}

	return true;
}
