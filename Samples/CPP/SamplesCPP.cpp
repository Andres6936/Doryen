/*
 * libtcod CPP samples
 * This code demonstrates various usages of libtcod modules
 * It's in the public domain.
 */

#include <memory>
#include <iostream>

#include "SampleRenderer.hpp"
#include "Functor/Interface/ISample.hpp"
#include "Functor/Bsp.hpp"
#include "Functor/Color.hpp"
#include "Functor/Fov.hpp"
#include "Functor/Image.hpp"
#include "Functor/Lines.hpp"
#include "Functor/Mouse.hpp"
#include "Functor/Name.hpp"
#include "Functor/Noise.hpp"
#include "Functor/Offscreen.hpp"
#include "Functor/Path.hpp"
#include "Functor/Sdl.hpp"

using namespace Doryen;

using namespace std::string_literals;

std::array<std::unique_ptr<Functor::ISample>, 11> configureExamples(std::reference_wrapper<Console> _console)
{
	// Although we declare a pointer to ISample, the reality is that
	// needed a instance of object derived of ISample, the aim is
	// allow save the pointer in an array and use its like a scene manager.
	std::array<std::unique_ptr<Functor::ISample>, 11> pointers;

	// Respect the sort original of the examples
	pointers[0] = std::make_unique<Functor::Color>("True colors"s, _console);;
	pointers[1] = std::make_unique<Functor::Offscreen>("Offscreen Console"s, _console);;
	pointers[2] = std::make_unique<Functor::Lines>("Line drawing"s, _console);;
	pointers[3] = std::make_unique<Functor::Noise>("Noise"s, _console);;
	pointers[4] = std::make_unique<Functor::FOV>("Field of view"s, _console);;
	pointers[5] = std::make_unique<Functor::Path>("Path finding"s, _console);;
	pointers[6] = std::make_unique<Functor::BSP>("Bsp toolkit"s, _console);;
	pointers[7] = std::make_unique<Functor::Image>("Image toolkit"s, _console);;
	pointers[8] = std::make_unique<Functor::Mouse>("Mouse support"s, _console);;
	pointers[9] = std::make_unique<Functor::Name>("Name generator"s, _console);;
	pointers[10] = std::make_unique<Functor::SDL>("SDL callback"s, _console);;

	return pointers;
}

int main(int argc, char* argv[])
{
	int curSample = 0; // index of the current sample
	bool first = true; // first time we render a sample

	const char* font = "Data/fonts/consolas10x10_gs_tc.png";
	int nbCharHoriz = 0, nbCharVertic = 0;
	int fullscreenWidth = 0;
	int fullscreenHeight = 0;
	bool fullscreen = false;
    int fontFlags = TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_TCOD, fontNewFlags = 0;
    bool creditsEnd = false;
    int cur_renderer = 0;

	if (fontFlags == 0)
	{ fontFlags = fontNewFlags; }

	Console console = Console();

	console.setCustomFont(font, fontFlags, nbCharHoriz, nbCharVertic);

	console.initRoot(80, 50, "libtcod C++ sample", fullscreen);

	std::array samples = configureExamples(std::ref(sampleConsole));

	while (console.isRunning())
	{
		if (!creditsEnd)
		{
			creditsEnd = Console::renderCredits(60, 43, false);
		}

		// print the list of samples
		for (int i = 0; i < samples.size(); i++)
		{
			if (i == curSample)
			{
				// set colors for currently selected sample
				console.setDefaultForeground(Palette::GRAY_WARN_1);
				console.setDefaultBackground(Palette::PRIMARY_LIGHT);
			}
			else
			{
				// set colors for other samples
				console.setDefaultForeground(Palette::GRAY_WARN_30);
				console.setDefaultBackground(Palette::GRAY_WARN_90);
			}

			// print the sample name
			console.write(2, 46 - (samples.size() - i), samples[i]->getName());
		}

		// print the help message
		console.setDefaultForeground(Palette::GRAY_WARN_30);
		console.write(50, 46,
				format("last frame : {>2.0f} ms ({} fps)",
						console.getLastFrameLength() * 1000,
						console.getFramePerSeconds()));

		console.write(50, 47,
				format("elapsed : {>5d} ms {>4.2f}s",
						console.getElapsedMilliseconds(),
						console.getElapsedSeconds()));

		console.write(2, 47,
				format("{c}{c} : select a sample",
						(char)TCOD_CHAR_ARROW_N,
						(char)TCOD_CHAR_ARROW_S));

		if (Console::isFullscreen())
		{
			console.write(2, 48, "ALT-ENTER : switch to windowed mode");
		}
		else
		{
			console.write(2, 48, "ALT-ENTER : switch to fullscreen mode");
		}

		const KeyCode _key = console.getKeyPressed().getKeyCode();
		const Mouse mouse = console.getMouseEvent();

		// render current sample
		samples[curSample]->render(_key, mouse);

		first = false;

		// blit the sample console on the root console
		sampleConsole.blit({ 0, 0 }, console, { SAMPLE_SCREEN_X, SAMPLE_SCREEN_Y });

		// update the game screen
		console.draw();

		if (_key == KeyCode::DOWN)
		{
			// down arrow : next sample
			curSample = (curSample + 1) % samples.size();
		}
		else if (_key == KeyCode::UP)
		{
			// up arrow : previous sample
			curSample--;
			if (curSample < 0)
			{ curSample = samples.size() - 1; }
		}
    }
}
