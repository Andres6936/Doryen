// Joan Andrés (@Andres6936) Github.

#include "App.hpp"

using namespace Sample;

void App::handleInput(const KeyCode key)
{
	if (key == KeyCode::UP)
	{
		scaleImage += 0.1f;
	}
	else if (key == KeyCode::DOWN)
	{
		scaleImage -= 0.1f;
	}
}

void App::render()
{
	Geometry::Size size = skull.getSize();

	while (console.isRunning())
	{
		console.clear();
		// Get the input of user
		handleInput(console.getKeyPressed().getKeyCode());

		skull.blit(console, { size.w / 2, size.h / 2 }, BlendModes::SET, scaleImage, scaleImage, 0);
		console.draw();
	}
}

int main()
{
	// Use of anonymous object
	App{}.render();
}