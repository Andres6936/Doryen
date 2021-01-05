// Joan Andrés (@Andres6936) Github.

#include "App.hpp"

using namespace Sample;

void App::render()
{
	Geometry::Size size = skull.getSize();

	while (console.isRunning())
	{
		skull.blit(console, { size.w / 2, size.h / 2 }, BlendModes::SET, 1, 1, 0);
		console.draw();
	}
}

int main()
{
	// Use of anonymous object
	App{}.render();
}