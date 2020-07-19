// Joan Andrés (@Andres6936) Github.

#include "Sdl.hpp"
#include "../SampleRenderer.hpp"

using namespace Doryen;

Functor::SDL::SDL(std::string _name, Console& _console) : ISample(_name, _console)
{

}

void Functor::SDL::render(KeyCode key, const Mouse& mouse)
{
	drawBackground();

	const std::string text = "The SDL callback gives you access to the screen surface so that you can alter the pixels one by one using SDL API or any API on top of SDL. SDL is used here to blur the sample console.\n\nHit TAB to enable/disable the callback. While enabled, it will be active on other samples too.\n\nNote that the SDL callback only works with SDL renderer.";

	sample.writeText({ 1, 3 }, { (int)sample.getWidth() - 1, 0 }, BackgroundFlag::SET, text);

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

void Functor::SDL::drawBackground() const
{
	// Use noise sample as background.
	// Rendering is done in SampleRenderer
	sample.setDefaultBackground(Palette::PRIMARY);
	sample.setDefaultForeground(Palette::BASE_LIGHTEST);

	sample.clear();
}
