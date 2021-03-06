// Joan Andrés (@Andres6936) Github.

#include <cstdint>

#include "Color.hpp"

using namespace Doryen;

Functor::Color::Color(Console& _console) : ISample("True colors", _console)
{
	prepareRandomCornerColors();
}

void Functor::Color::render(KeyCode key, const Mouse& mouse)
{
	const std::uint8_t TOP_LEFT = 0;
	const std::uint8_t TOP_RIGHT = 1;
	const std::uint8_t BOTTOM_LEFT = 2;
	const std::uint8_t BOTTOM_RIGHT = 3;

	static int dirr[4] = { 1, -1, 1, 1 }, dirg[4] = { 1, -1, -1, 1 }, dirb[4] = { 1, 1, 1, -1 };

	// Slightly modify the corner colors
	for (int c = 0; c < cornerColors.size(); c++)
	{
		// move each corner color
		int component = Random::Number::nextInteger(0, 2);
		switch (component)
		{
		case 0 :
			cornerColors[c].addRed(5 * dirr[c]);
			if (cornerColors[c].getRed() == 255)
			{ dirr[c] = -1; }
			else if (cornerColors[c].getRed() == 0)
			{ dirr[c] = 1; }
			break;
		case 1 :
			cornerColors[c].g += 5 * dirg[c];
			if (cornerColors[c].g == 255)
			{ dirg[c] = -1; }
			else if (cornerColors[c].g == 0)
			{ dirg[c] = 1; }
			break;
		case 2 :
			cornerColors[c].b += 5 * dirb[c];
			if (cornerColors[c].b == 255)
			{ dirb[c] = -1; }
			else if (cornerColors[c].b == 0)
			{ dirb[c] = 1; }
			break;
		}
	}

	// ==== scan the whole screen, interpolating corner colors ====
	for (int x = 0; x < sample.getWidth(); x++)
	{
		float xcoef = (float)(x) / (sample.getWidth() - 1);
		// get the current column top and bottom colors
		Doryen::Color top = Doryen::Color::lerp(cornerColors[TOP_LEFT], cornerColors[TOP_RIGHT], xcoef);
		Doryen::Color bottom = Doryen::Color::lerp(cornerColors[BOTTOM_LEFT], cornerColors[BOTTOM_RIGHT], xcoef);
		for (int y = 0; y < sample.getHeight(); y++)
		{
			float ycoef = (float)(y) / (sample.getHeight() - 1);
			// get the current cell color
			Doryen::Color curColor = Doryen::Color::lerp(top, bottom, ycoef);
			sample.setCellBackgroundColor(x, y, curColor, Doryen::BlendModes::SET);
		}
	}

	// ==== print the text with a random color ====
	// get the background color at the text position
	Doryen::Color textColor = sample.getCellBackgroundColor(sample.getWidth() / 2, 5);
	// and invert it
	textColor.setRed(255 - textColor.getRed());
	textColor.g = 255 - textColor.g;
	textColor.b = 255 - textColor.b;
	// put random text (for performance tests)
	for (int x = 0; x < sample.getWidth(); x++)
	{
		for (int y = 0; y < sample.getHeight(); y++)
		{
			int c;
			Doryen::Color col = sample.getCellBackgroundColor(x, y);
			col.copy(Doryen::Color::lerp(col, Doryen::Palette::GRAY_WARN_90, 0.5f));
			// use colored character 255 on first and last lines
			if (y == 0 || y == sample.getHeight() - 1)
			{
				c = 255;
			}
			else
			{
				c = Random::Number::nextInteger('a', 'z');
			}

			sample.setForegroundColor(col);
			sample.writeChar(x, y, c, Doryen::BlendModes::NONE);
		}
	}
	sample.setForegroundColor(textColor);
	// the background behind the text is slightly darkened using the BKGND_MULTIPLY flag
	sample.setBackgroundColor(Doryen::Palette::GRAY_WARN_30);
	sample.writeWrapText(2, (sample.getHeight() / 2) - 2, sample.getWidth() - 4, 4,
			BlendModes::MULTIPLY, 1,
			"The Doryen library uses 24 bits colors, for both background and foreground.");
}

void Functor::Color::prepareRandomCornerColors()
{
	cornerColors[0].copy(Doryen::Color(50, 40, 150));
	cornerColors[1].copy(Doryen::Color(240, 85, 5));
	cornerColors[2].copy(Doryen::Color(50, 35, 240));
	cornerColors[3].copy(Doryen::Color(10, 200, 130));
}
