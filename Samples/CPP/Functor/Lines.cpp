// Joan Andrés (@Andres6936) Github.

#include "Lines.hpp"
#include "../LineListener.hpp"

using namespace Doryen;

Functor::Lines::Lines(Console& _console) : ISample("Line drawing", _console)
{

}

void Functor::Lines::render(KeyCode key, const Mouse& mouse)
{
	static Console bk(sample.getWidth(), sample.getHeight()); // colored background
	static bool init = false;

	static const char* flagNames[] = {
			"TCOD_BKGND_NONE",
			"TCOD_BKGND_SET",
			"TCOD_BKGND_MULTIPLY",
			"TCOD_BKGND_LIGHTEN",
			"TCOD_BKGND_DARKEN",
			"TCOD_BKGND_SCREEN",
			"TCOD_BKGND_COLOR_DODGE",
			"TCOD_BKGND_COLOR_BURN",
			"TCOD_BKGND_ADD",
			"TCOD_BKGND_ADDALPHA",
			"TCOD_BKGND_BURN",
			"TCOD_BKGND_OVERLAY",
			"TCOD_BKGND_ALPHA",
			"TCOD_BKGND_DEFAULT"
	};

	if (key == KeyCode::ENTER or key == KeyCode::KP_ENTER)
	{
		// switch to the next blending mode
		backFlag = switchBackgroundFlag(backFlag);
	}

	// TODO: Here, I break code, Fix. See Git

	if (!init)
	{
		// initialize the colored background
		for (int x = 0; x < sample.getWidth(); x++)
		{
			for (int y = 0; y < sample.getHeight(); y++)
			{
				Doryen::Color col;
				col.r = (uint8)(x * 255 / (sample.getWidth() - 1));
				col.g = (uint8)((x + y) * 255 / (sample.getWidth() - 1 + sample.getHeight() - 1));
				col.b = (uint8)(y * 255 / (sample.getHeight() - 1));
				bk.setCellBackgroundColor(x, y, col, Doryen::BlendModes::SET);
			}
		}
		init = true;
	}

	// blit the background
	bk.blit({ 0, 0 }, sample, { 0, 0 });
	// render the gradient
	int recty = (int)((sample.getHeight() - 2) *
					  ((1.0f + cosf(sample.getElapsedSeconds())) / 2.0f));
	for (int x = 0; x < sample.getWidth(); x++)
	{
		Doryen::Color col;
		col.r = (uint8)(x * 255 / sample.getWidth());
		col.g = (uint8)(x * 255 / sample.getWidth());
		col.b = (uint8)(x * 255 / sample.getWidth());

		sample.setCellBackgroundColor(x, recty, col, backFlag);
		sample.setCellBackgroundColor(x, recty + 1, col, backFlag);
		sample.setCellBackgroundColor(x, recty + 2, col, backFlag);
	}
	// calculate the segment ends
	float angle = sample.getElapsedSeconds() * 2.0f;
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	int xo = (int)(sample.getWidth() / 2 * (1 + cosAngle));
	int yo = (int)(sample.getHeight() / 2 + sinAngle * sample.getWidth() / 2);
	int xd = (int)(sample.getWidth() / 2 * (1 - cosAngle));
	int yd = (int)(sample.getHeight() / 2 - sinAngle * sample.getWidth() / 2);

	// render the line
	RenderLine listener{ sample };

	Doryen::Algorithms::Line objLine = Doryen::Algorithms::Line();

	objLine.line(xo, yo, xd, yd, listener);

	// print the current flag
	sample.write(2, 2, format("{} (ENTER to change)", flagNames[(int)backFlag]));
}


Doryen::BlendModes Functor::Lines::switchBackgroundFlag(Doryen::BlendModes flag)
{
	switch (flag)
	{
	case BlendModes::NONE:
		return BlendModes::SET;

	case BlendModes::SET:
		return BlendModes::MULTIPLY;

	case BlendModes::MULTIPLY:
		return BlendModes::LIGHTEN;

	case BlendModes::LIGHTEN:
		return BlendModes::DARKEN;

	case BlendModes::DARKEN:
		return BlendModes::SCREEN;

	case BlendModes::SCREEN:
		return BlendModes::COLOR_DODGE;

	case BlendModes::COLOR_DODGE:
		return BlendModes::COLOR_BURN;

	case BlendModes::COLOR_BURN:
		return BlendModes::ADD;

	case BlendModes::ADD:
		return BlendModes::ADDA;

	case BlendModes::ADDA:
		return BlendModes::BURN;

	case BlendModes::BURN:
		return BlendModes::OVERLAY;

	case BlendModes::OVERLAY:
		return BlendModes::ALPH;

	case BlendModes::ALPH:
		return BlendModes::DEFAULT;

	case BlendModes::DEFAULT:
		return BlendModes::NONE;

	default:
		return BlendModes::SET;
	}
}
