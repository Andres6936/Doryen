// Joan Andrés (@Andres6936) Github.

#include "Sdl.hpp"
#include "../SampleRenderer.hpp"

using namespace Doryen;

Functor::SDL::SDL(std::string _name, const Console& _console) : ISample(_name), sample(_console)
{

}

void Functor::SDL::operator()(KeyCode key, const Mouse& mouse)
{
	static bool first = true;

	if (first)
	{
		first = false;

		Doryen::Platform::setFps(30); /* limited to 30 fps */
		// use noise sample as background. rendering is done in SampleRenderer
		sample.setDefaultBackground(Doryen::Color::lightBlue);
		sample.setDefaultForeground(Doryen::Color::white);
		sample.clear();
		sample.printRectEx(sample.getWidth() / 2, 3, sample.getWidth(), 0, TCOD_BKGND_NONE, TCOD_CENTER,
				"The SDL callback gives you access to the screen surface so that you can alter the pixels one by one using SDL API or any API on top of SDL. SDL is used here to blur the sample console.\n\nHit TAB to enable/disable the callback. While enabled, it will be active on other samples too.\n\nNote that the SDL callback only works with SDL renderer.");
	}
	if (key == KeyCode::TAB)
	{
		sdl_callback_enabled = !sdl_callback_enabled;

		if (sdl_callback_enabled)
		{
			Doryen::Platform::registerSDLRenderer(new SampleRenderer());
		}
		else
		{
			Doryen::Platform::registerSDLRenderer(NULL);
			// we want libtcod to redraw the sample console even if nothing has changed in it
			Doryen::Console::root->setDirty(SAMPLE_SCREEN_X, SAMPLE_SCREEN_Y, SAMPLE_SCREEN_WIDTH,
					SAMPLE_SCREEN_HEIGHT);
		}
	}
}
