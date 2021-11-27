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

	// Satisfies post-condition
	// Create the buffer for render
	// characters in the console
	createBuffer();

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

	// Calling clear before drawing anything is mandatory, otherwise
	// the contents from previous frames will be present behind anything
	// you draw. The only exception is when you cover the entire window
	// with what you draw, so that no pixel is not drawn to. In this case
	// you can avoid calling clear (although it won't have a noticeable
	// impact on performance).
	window.clear(sf::Color::White);

	// Calling display is also mandatory, it takes what was drawn since
	// the last call to display and displays it on the window. Indeed,
	// things are not drawn directly to the window, but to a hidden buffer.
	// This buffer is then copied to the window when you call display
	// -- this is called double-buffering.
	window.display();

	// This clear/draw/display cycle is the only good way to draw things.
	// Don't try other strategies, such as keeping pixels from the previous
	// frame, "erasing" pixels, or drawing once and calling display multiple
	// times. You'll get strange results due to double-buffering.
	//Modern graphics hardware and APIs are really made for repeated
	// clear/draw/display cycles where everything is completely refreshed
	// at each iteration of the main loop. Don't be scared to draw 1000
	// sprites 60 times per second, you're far below the millions of triangles
	// that your computer can handle.
}

void Doryen::SFML::loadFont()
{
	if (!texture.loadFromFile(getFontfile()))
	{
		// Throw Exception
		throw std::runtime_error("The type of image cannot will be opened.\n");
	}

	const auto [width, height] = texture.getSize();
	setFontWidth(width / getFontCharHorizontalSize());
	setFontHeight(height / getFontCharVerticalSize());

	// Satisfies post-condition
	// allocated bool array for colored flags
	createTablesOfCharacters();

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
