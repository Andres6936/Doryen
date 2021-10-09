#include "Doryen/Renderer/Sfml.hpp"

using namespace Doryen;

SFML::~SFML()
{
	window.close();
}

void Doryen::SFML::onRenderer()
{
	const std::uint32_t width = getWidth() * getFontCharVerticalSize();
	const std::uint32_t height = getHeight() * getFontCharHorizontalSize();

	window.create(sf::VideoMode(width, height), "SFML Renderer");
}

void Doryen::SFML::draw()
{
	// check all the window's events that were triggered since the last iteration of the loop
	sf::Event event;
	while (window.pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed)
		{
			setRunning(false);
		}
	}
}

void Doryen::SFML::loadFont()
{

}

void Doryen::SFML::showCursor(bool visible)
{

}

void Doryen::SFML::setWindowTitle(const std::string& _title)
{

}

void Doryen::SFML::registerCallback(std::unique_ptr<CallbackRender> _callback)
{

}
