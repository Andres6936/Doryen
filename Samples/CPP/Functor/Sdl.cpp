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
	drawText();

	if (key == KeyCode::TAB)
	{
		renderCallback = not renderCallback;

		if (renderCallback)
		{
			sample.registerCallback(std::make_unique<SampleRenderer>());
		}
		else
		{
			sample.unregisterCallback();
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

void Functor::SDL::drawText() const
{
	sample.writeText({ 1, 3 }, { (int)sample.getWidth() - 1, 0 }, BackgroundFlag::SET, text);
}
