// Joan Andrés (@Andres6936) Github.

#include "Color.hpp"

using namespace Doryen;

void Functor::Color::operator()(KeyCode key, const Mouse& mouse)
{
	enum
	{
		TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT
	};
	static Doryen::Color cols[4] = { Doryen::Color(50, 40, 150), Doryen::Color(240, 85, 5),
									 Doryen::Color(50, 35, 240),
									 Doryen::Color(10, 200, 130) }; // random corner colors
	static int dirr[4] = { 1, -1, 1, 1 }, dirg[4] = { 1, -1, -1, 1 }, dirb[4] = { 1, 1, 1, -1 };

	// ==== slighty modify the corner colors ====
	for (int c = 0; c < 4; c++)
	{
		// move each corner color
		int component = TCODRandom::getInstance()->getInt(0, 2);
		switch (component)
		{
		case 0 :
			cols[c].r += 5 * dirr[c];
			if (cols[c].r == 255)
			{ dirr[c] = -1; }
			else if (cols[c].r == 0)
			{ dirr[c] = 1; }
			break;
		case 1 :
			cols[c].g += 5 * dirg[c];
			if (cols[c].g == 255)
			{ dirg[c] = -1; }
			else if (cols[c].g == 0)
			{ dirg[c] = 1; }
			break;
		case 2 :
			cols[c].b += 5 * dirb[c];
			if (cols[c].b == 255)
			{ dirb[c] = -1; }
			else if (cols[c].b == 0)
			{ dirb[c] = 1; }
			break;
		}
	}

	// ==== scan the whole screen, interpolating corner colors ====
	for (int x = 0; x < secondary.getWidth(); x++)
	{
		float xcoef = (float)(x) / (secondary.getWidth() - 1);
		// get the current column top and bottom colors
		Doryen::Color top = Doryen::Color::lerp(cols[TOPLEFT], cols[TOPRIGHT], xcoef);
		Doryen::Color bottom = Doryen::Color::lerp(cols[BOTTOMLEFT], cols[BOTTOMRIGHT], xcoef);
		for (int y = 0; y < secondary.getHeight(); y++)
		{
			float ycoef = (float)(y) / (secondary.getHeight() - 1);
			// get the current cell color
			Doryen::Color curColor = Doryen::Color::lerp(top, bottom, ycoef);
			secondary.setCharBackground(x, y, curColor, Doryen::BackgroundFlag::SET);
		}
	}

	// ==== print the text with a random color ====
	// get the background color at the text position
	Doryen::Color textColor = secondary.getCharBackground(secondary.getWidth() / 2, 5);
	// and invert it
	textColor.r = 255 - textColor.r;
	textColor.g = 255 - textColor.g;
	textColor.b = 255 - textColor.b;
	// put random text (for performance tests)
	for (int x = 0; x < secondary.getWidth(); x++)
	{
		for (int y = 0; y < secondary.getHeight(); y++)
		{
			int c;
			Doryen::Color col = secondary.getCharBackground(x, y);
			col = Doryen::Color::lerp(col, Doryen::Color::black, 0.5f);
			// use colored character 255 on first and last lines
			if (y == 0 || y == secondary.getHeight() - 1)
			{
				c = 255;
			}
			else
			{
				c = TCODRandom::getInstance()->getInt('a', 'z');
			}

			secondary.setDefaultForeground(col);
			secondary.putChar(x, y, c, Doryen::BackgroundFlag::NONE);
		}
	}
	secondary.setDefaultForeground(textColor);
	// the background behind the text is slightly darkened using the BKGND_MULTIPLY flag
	secondary.setDefaultBackground(Doryen::Color::grey);
	secondary.printRectEx(secondary.getWidth() / 2, 5, secondary.getWidth() - 2, secondary.getHeight() - 1,
			TCOD_BKGND_MULTIPLY, TCOD_CENTER,
			"The Doryen library uses 24 bits colors, for both background and foreground.");
}
