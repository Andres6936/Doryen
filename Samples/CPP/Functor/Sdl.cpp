// Joan Andrés (@Andres6936) Github.

#include <iostream>
#include "Sdl.hpp"
#include "../SampleRenderer.hpp"

using namespace Doryen;

Functor::SDL::SDL(Console& _console) : ISample("SDL callback", _console)
{

}

void Functor::SDL::render(KeyCode key, const Mouse& mouse)
{
	drawBackground();
	drawText();

	if (key == KeyCode::TAB)
	{
		// Toggle the variable
		registerCallback = not registerCallback;

		if (registerCallback)
		{
			if (not renderingCallback)
			{
				sample.registerCallback(std::make_unique<SampleRenderer>(sample));
				renderingCallback = true;
			}
		}
		else
		{
			sample.unregisterCallback();
			renderingCallback = false;
		}
	}
}

void Functor::SDL::drawBackground() const
{
	// Use noise sample as background.
	// Rendering is done in SampleRenderer
	sample.setBackgroundColor(Palette::PRIMARY);
	sample.setForegroundColor(Palette::BASE_LIGHTEST);

	sample.clear();
}

void Functor::SDL::drawText() const
{
	sample.writeWrapText({ 1, 3 }, { (int)sample.getWidth() - 1, 0 }, BlendModes::SET, text);
}
