/*
 * libtcod CPP samples
 * This code demonstrates various usages of libtcod modules
 * It's in the public domain.
 */

#include <chrono>
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
#include "Functor/Noise.hpp"
#include "Functor/Offscreen.hpp"
#include "Functor/Path.hpp"
#include "Functor/Sdl.hpp"

using namespace Doryen;

std::array<std::unique_ptr<Functor::ISample>, 10> configureExamples(std::reference_wrapper<Console> _console)
{
	// Although we declare a pointer to ISample, the reality is that
	// needed a instance of object derived of ISample, the aim is
	// allow save the pointer in an array and use its like a scene manager.
	std::array<std::unique_ptr<Functor::ISample>, 10> pointers;

	// Respect the sort original of the examples
	pointers[0] = std::make_unique<Functor::Color>(_console);;
	pointers[1] = std::make_unique<Functor::Offscreen>(_console);;
	pointers[2] = std::make_unique<Functor::Lines>(_console);;
	pointers[3] = std::make_unique<Functor::Noise>(_console);;
	pointers[4] = std::make_unique<Functor::FOV>(_console);;
	pointers[5] = std::make_unique<Functor::Path>(_console);;
	pointers[6] = std::make_unique<Functor::BSP>(_console);;
	pointers[7] = std::make_unique<Functor::Image>(_console);;
	pointers[8] = std::make_unique<Functor::Mouse>(_console);;
	pointers[9] = std::make_unique<Functor::SDL>(_console);;

	return pointers;
}

void drawHelpMessages(Console& console)
{
	console.setForegroundColor(Palette::GRAY_WARN_30);

	const std::string lastFrame = format("Last Frame : {>2.0f} ms ({} fps)",
			console.getLastFrameLength() * 1000,
			console.getFramePerSeconds());

	const std::string timeElapsed = format("Elapsed : {>5d} ms {>4.2f}s",
			console.getElapsedMilliseconds(),
			console.getElapsedSeconds());

	console.writeString(50, 46, lastFrame);
	console.writeString(50, 47, timeElapsed);

	console.writeString(2, 47, format("{c}{c} : Change of Sample", (char)24, (char)25));
}

int main(int argc, char* argv[])
{
	int curSample = 0; // index of the current sample

	const char* font = "Data/fonts/consolas10x10_gs_tc.png";
	int nbCharHoriz = 0, nbCharVertic = 0;

	bool fullscreen = false;

	const std::uint8_t FONT_TYPE_GREYSCALE = 4;
	const std::uint8_t FONT_LAYOUT_TCOD = 8;

	const auto beginTime = std::chrono::high_resolution_clock::now();

	// Size for default, 80 x 50
	Console console{};
	console.setWindowTitle("Doryen Showcase");
	console.setCustomFont(font, FONT_TYPE_GREYSCALE | FONT_LAYOUT_TCOD, nbCharHoriz, nbCharVertic);

	// The offscreen console in which the samples are rendered
	Console innerConsole(SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT);

	const auto endTime = std::chrono::high_resolution_clock::now();

	std::cout << "Total time : " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count()
			  << " microseconds\n";

	std::array samples = configureExamples(std::ref(innerConsole));

	while (console.isRunning())
	{
		// print the list of samples
		for (int i = 0; i < samples.size(); i++)
		{
			if (i == curSample)
			{
				// set colors for currently selected sample
				console.setForegroundColor(Palette::GRAY_WARN_1);
				console.setBackgroundColor(Palette::PRIMARY_LIGHT);
			}
			else
			{
				// set colors for other samples
				console.setForegroundColor(Palette::GRAY_WARN_30);
				console.setBackgroundColor(Palette::GRAY_WARN_90);
			}

			// print the sample name
			console.writeString(2, 46 - (samples.size() - i), samples[i]->getName());
		}

		drawHelpMessages(console);

		const KeyCode _key = console.getKeyPressed().getKeyCode();
		const Mouse mouse = console.getMouseEvent();

		// render current sample
		samples[curSample]->render(_key, mouse);

		// blit the sample console on the root console
		innerConsole.blit({ 0, 0 }, console, { SAMPLE_SCREEN_X, SAMPLE_SCREEN_Y });

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
