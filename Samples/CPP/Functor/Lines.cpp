// Joan Andrés (@Andres6936) Github.

#include "Lines.hpp"
#include "../LineListener.hpp"

using namespace Doryen;

Functor::Lines::Lines(std::string _name, std::reference_wrapper<Console> _console) : ISample(_name), sample(_console)
{

}

void Functor::Lines::render(KeyCode key, const Mouse& mouse)
{
	static Console bk(sample.get().getWidth(), sample.get().getHeight()); // colored background
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
		switchBackgroundFlag(backFlag);
	}

	// TODO: Here, I break code, Fix. See Git

	if (!init)
	{
		// initialize the colored background
		for (int x = 0; x < sample.get().getWidth(); x++)
		{
			for (int y = 0; y < sample.get().getHeight(); y++)
			{
				Doryen::Color col;
				col.r = (uint8)(x * 255 / (sample.get().getWidth() - 1));
				col.g = (uint8)((x + y) * 255 / (sample.get().getWidth() - 1 + sample.get().getHeight() - 1));
				col.b = (uint8)(y * 255 / (sample.get().getHeight() - 1));
				bk.setCharBackground(x, y, col, Doryen::BackgroundFlag::SET);
			}
		}
		init = true;
	}

	// blit the background
	bk.blit({ 0, 0 }, sample, { 0, 0 });
	// render the gradient
	int recty = (int)((sample.get().getHeight() - 2) *
					  ((1.0f + cosf(Doryen::Platform::getElapsedSeconds())) / 2.0f));
	for (int x = 0; x < sample.get().getWidth(); x++)
	{
		Doryen::Color col;
		col.r = (uint8)(x * 255 / sample.get().getWidth());
		col.g = (uint8)(x * 255 / sample.get().getWidth());
		col.b = (uint8)(x * 255 / sample.get().getWidth());

		sample.get().setCharBackground(x, recty, col, backFlag);
		sample.get().setCharBackground(x, recty + 1, col, backFlag);
		sample.get().setCharBackground(x, recty + 2, col, backFlag);
	}
	// calculate the segment ends
	float angle = Doryen::Platform::getElapsedSeconds() * 2.0f;
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	int xo = (int)(sample.get().getWidth() / 2 * (1 + cosAngle));
	int yo = (int)(sample.get().getHeight() / 2 + sinAngle * sample.get().getWidth() / 2);
	int xd = (int)(sample.get().getWidth() / 2 * (1 - cosAngle));
	int yd = (int)(sample.get().getHeight() / 2 - sinAngle * sample.get().getWidth() / 2);

	// render the line
	RenderLine listener;

	Doryen::Algorithms::Line objLine = Doryen::Algorithms::Line();

	objLine.line(xo, yo, xd, yd, listener);

	// print the current flag
	sample.get().print(2, 2, format("{} (ENTER to change)", flagNames[(int)backFlag]));
}


Doryen::BackgroundFlag Functor::Lines::switchBackgroundFlag(Doryen::BackgroundFlag flag)
{
	using namespace Doryen;

	switch (flag)
	{
	case BackgroundFlag::NONE:
		flag = BackgroundFlag::SET;
		break;

	case BackgroundFlag::SET:
		flag = BackgroundFlag::MULTIPLY;
		break;

	case BackgroundFlag::MULTIPLY:
		flag = BackgroundFlag::LIGHTEN;
		break;

	case BackgroundFlag::LIGHTEN:
		flag = BackgroundFlag::DARKEN;
		break;

	case BackgroundFlag::DARKEN:
		flag = BackgroundFlag::SCREEN;
		break;

	case BackgroundFlag::SCREEN:
		flag = BackgroundFlag::COLOR_DODGE;
		break;

	case BackgroundFlag::COLOR_DODGE:
		flag = BackgroundFlag::COLOR_BURN;
		break;

	case BackgroundFlag::COLOR_BURN:
		flag = BackgroundFlag::ADD;
		break;

	case BackgroundFlag::ADD:
		flag = BackgroundFlag::ADDA;
		break;

	case BackgroundFlag::ADDA:
		flag = BackgroundFlag::BURN;
		break;

	case BackgroundFlag::BURN:
		flag = BackgroundFlag::OVERLAY;
		break;

	case BackgroundFlag::OVERLAY:
		flag = BackgroundFlag::ALPH;
		break;

	case BackgroundFlag::ALPH:
		flag = BackgroundFlag::DEFAULT;
		break;

	case BackgroundFlag::DEFAULT:
		flag = BackgroundFlag::NONE;
		break;
	}
}
