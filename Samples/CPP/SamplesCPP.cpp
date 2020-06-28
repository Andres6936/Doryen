/*
 * libtcod CPP samples
 * This code demonstrates various usages of libtcod modules
 * It's in the public domain.
 */

#include <memory>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <functional>

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

std::array<std::shared_ptr<Functor::ISample>, 11> configureExamples(std::reference_wrapper<Console> _console)
{
	// Although we declare a pointer to ISample, the reality is that
	// needed a instance of object derived of ISample, the aim is
	// allow save the pointer an array and use its like a scene manager.
	using SharedPointer = typename std::shared_ptr<Functor::ISample>;

	std::array<SharedPointer, 11> pointers;

	// Respect the sort original of the examples
	pointers[0] = std::make_shared<Functor::Color>("  True colors        "s, _console);;
	pointers[1] = std::make_shared<Functor::Offscreen>("  Offscreen console  "s, _console);;
	pointers[2] = std::make_shared<Functor::Lines>("  Line drawing       "s, _console);;
	pointers[3] = std::make_shared<Functor::Noise>("  Noise              "s, _console);;
	pointers[4] = std::make_shared<Functor::FOV>("  Field of view      "s, _console);;
	pointers[5] = std::make_shared<Functor::Path>("  Path finding       "s, _console);;
	pointers[6] = std::make_shared<Functor::BSP>("  Bsp toolkit        "s, _console);;
	pointers[7] = std::make_shared<Functor::Image>("  Image toolkit      "s, _console);;
	pointers[8] = std::make_shared<Functor::Mouse>("  Mouse support      "s, _console);;
	pointers[9] = std::make_shared<Functor::Name>("  Name generator     "s, _console);;
	pointers[10] = std::make_shared<Functor::SDL>("  SDL callback       "s, _console);;

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
	TCOD_renderer_t renderer = TCOD_RENDERER_SDL;
	bool fullscreen = false;
    int fontFlags = TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_TCOD, fontNewFlags = 0;
    bool creditsEnd = false;
    int cur_renderer = 0;
    static const char *renderer_name[TCOD_NB_RENDERERS] = {
            "F1 GLSL   ", "F2 OPENGL ", "F3 SDL    "
    };

    // initialize the root console (open the game window)
    for ( int j = 1; j < argc; j++ )
    {
        if ( strcmp( argv[ j ], "-font" ) == 0 && j + 1 < argc )
        {
            j++;
            font = argv[ j ];
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-font-nb-char" ) == 0 && j + 2 < argc )
        {
            j++;
            nbCharHoriz = atoi( argv[ j ] );
            j++;
            nbCharVertic = atoi( argv[ j ] );
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-fullscreen-resolution" ) == 0 && j + 2 < argc )
        {
            j++;
            fullscreenWidth = atoi( argv[ j ] );
            j++;
            fullscreenHeight = atoi( argv[ j ] );
        }
        else if ( strcmp( argv[ j ], "-renderer" ) == 0 && j + 1 < argc )
        {
            j++;
            renderer = ( TCOD_renderer_t ) atoi( argv[ j ] );
        }
        else if ( strcmp( argv[ j ], "-fullscreen" ) == 0 )
        {
            fullscreen = true;
        }
        else if ( strcmp( argv[ j ], "-font-in-row" ) == 0 )
        {
            fontNewFlags |= TCOD_FONT_LAYOUT_ASCII_INROW;
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-font-greyscale" ) == 0 )
        {
            fontNewFlags |= TCOD_FONT_TYPE_GREYSCALE;
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-font-tcod" ) == 0 )
        {
            fontNewFlags |= TCOD_FONT_LAYOUT_TCOD;
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-help" ) == 0 || strcmp( argv[ j ], "-?" ) == 0 )
        {
            printf( "options :\n" );
            printf( "-font <filename> : use a custom font\n" );
            printf( "-font-nb-char <nb_char_horiz> <nb_char_vertic> : number of characters in the font\n" );
            printf( "-font-in-row : the font layout is in row instead of columns\n" );
            printf( "-font-tcod : the font uses TCOD layout instead of ASCII\n" );
            printf( "-font-greyscale : antialiased font using greyscale bitmap\n" );
            printf( "-fullscreen : start in fullscreen\n" );
            printf( "-fullscreen-resolution <screen_width> <screen_height> : force fullscreen resolution\n" );
            printf( "-renderer <num> : set renderer. 0 : GLSL 1 : OPENGL 2 : SDL\n" );
            exit( 0 );
        }
		else
		{
			// ignore parameter
		}
	}

	if (fontFlags == 0)
	{ fontFlags = fontNewFlags; }


	if (fullscreenWidth > 0)
	{
		Doryen::Platform::forceFullscreenResolution(fullscreenWidth, fullscreenHeight);
	}

	Console console = Console();

	console.setCustomFont(font, fontFlags, nbCharHoriz, nbCharVertic);

	console.initRoot(80, 50, "libtcod C++ sample", fullscreen, renderer);

	std::array<std::shared_ptr<Functor::ISample>, 11> samples = configureExamples(std::ref(sampleConsole));

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
				console.setDefaultForeground(Doryen::Color::white);
				console.setDefaultBackground(Doryen::Color::lightBlue);
			}
			else
			{
				// set colors for other samples
				console.setDefaultForeground(Doryen::Color::grey);
				console.setDefaultBackground(Doryen::Color::black);
			}

			// print the sample name
			console.print(2, 46 - (samples.size() - i), samples[i]->getName());
		}

		// print the help message
		console.setDefaultForeground(Doryen::Color::grey);
		console.print(50, 46,
				format("last frame : {>2.0f} ms ({} fps)",
						console.getLastFrameLength() * 1000,
						console.getFramePerSeconds()));

		console.print(50, 47,
				format("elapsed : {>5d} ms {>4.2f}s",
						Doryen::Platform::getElapsedMilli(),
						Doryen::Platform::getElapsedSeconds()));

		console.print(2, 47,
				format("{c}{c} : select a sample",
						(char)TCOD_CHAR_ARROW_N,
						(char)TCOD_CHAR_ARROW_S));

		if (Console::isFullscreen())
		{
			console.print(2, 48, "ALT-ENTER : switch to windowed mode");
		}
		else
		{
			console.print(2, 48, "ALT-ENTER : switch to fullscreen mode");
		}

		const KeyCode _key = console.getKeyPressed().getKeyCode();
		const Mouse mouse = console.getMouseEvent();

		// render current sample
		samples[curSample]->render(_key, mouse);

		first = false;

		// blit the sample console on the root console
		sampleConsole.blit({ 0, 0 }, console, { SAMPLE_SCREEN_X, SAMPLE_SCREEN_Y });

		// erase the renderer in debug mode (needed because the root console is not cleared each frame)
		console.print(1, 1, "        ");
#ifndef NO_SDL_SAMPLE
		if (sdl_callback_enabled)
		{
			// we want libtcod to redraw the sample console even if nothing has changed in it
			console.setDirty(SAMPLE_SCREEN_X, SAMPLE_SCREEN_Y, SAMPLE_SCREEN_WIDTH,
					SAMPLE_SCREEN_HEIGHT);
		}
#endif
		/* display renderer list and current renderer */
		cur_renderer = Doryen::Platform::getRenderer();

		console.setDefaultForeground(Doryen::Color::grey);
		console.setDefaultBackground(Doryen::Color::black);
		console.printEx(42, 46 - (TCOD_NB_RENDERERS + 1), TCOD_BKGND_SET, TCOD_LEFT, "Renderer :");

		for (int i = 0; i < TCOD_NB_RENDERERS; i++)
		{
			if (i == cur_renderer)
			{
				/* set colors for current renderer */
				console.setDefaultForeground(Doryen::Color::white);
				console.setDefaultBackground(Doryen::Color::lightBlue);
			}
			else
			{
				/* set colors for other renderer */
				console.setDefaultForeground(Doryen::Color::grey);
				console.setDefaultBackground(Doryen::Color::black);
			}
			console.printEx(42, 46 - (TCOD_NB_RENDERERS - i), TCOD_BKGND_SET, TCOD_LEFT,
					renderer_name[i]);
		}

		// update the game screen
		Doryen::Console::flush();

		if (_key == KeyCode::DOWN)
		{
			// down arrow : next sample
			curSample = (curSample + 1) % samples.size();
			first = true;
		}
		else if (_key == KeyCode::UP)
		{
			// up arrow : previous sample
			curSample--;
			if (curSample < 0)
			{ curSample = samples.size() - 1; }
			first = true;
		}
		else if (_key == KeyCode::ENTER and console.getKeyPressed().isLeftAltPressed())
		{
			// ALT-ENTER : switch fullscreen
			Doryen::Console::setWindowInFullscreen();
#ifdef TCOD_LINUX
		}
		else if (_key == KeyCode::PRINT_SCREEN)
		{
#else
			} else if ( key.vk == TCODK_PRINTSCREEN ) {
#endif
		}
		else if (_key == KeyCode::F1)
		{
			// switch renderers with F1,F2,F3
			Doryen::Platform::setRenderer(TCOD_RENDERER_GLSL);
		}
		else if (_key == KeyCode::F2)
		{
			Doryen::Platform::setRenderer(TCOD_RENDERER_OPENGL);
		}
		else if (_key == KeyCode::F3)
		{
			Doryen::Platform::setRenderer(TCOD_RENDERER_SDL);
		}
    }

    return 0;
}
