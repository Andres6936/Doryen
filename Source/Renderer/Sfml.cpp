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

	screen.create(width, height);
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
	screen.clear(sf::Color::Red);
	window.clear(sf::Color::White);

	for (int y = 0; y < getHeight(); ++y)
	{
		for (int x = 0; x < getWidth(); ++x)
		{
			// Character to draw
			Char character = front[x + getWidth() * y];

			// Previous character drawed
			Char previousCharacter = back[x + getWidth() * y];

			sf::Rect<std::uint32_t> sourceRect;
			sf::Rect<std::uint32_t> destinRect;

			if (character.getCharacterFont() == -1)
			{
				character.setCharacterFont(
						getCharacterInLayoutCharacteres(character.getCharacter()));
			}

			bool changed = false;

			if (not character.equals(previousCharacter) or
				isCharacterUpdated(character.getCharacter()))
			{
				changed = true;
			}

			if (changed){
				destinRect.left = x * getFontWidth();
				destinRect.top = y * getFontHeight();

				destinRect.width = getFontWidth();
				destinRect.height = getFontHeight();

				// Avoid draw the space character (A empty character)
				if (character.getCharacter() == ' ')
				{
					// Update the character and continue with the next Char
					setCharacterInBufferAt(x + getWidth() * y, character);
					continue;
				}

				sourceRect.left = (character.getCharacterFont()
						% getFontCharHorizontalSize()) * getFontWidth();
				sourceRect.top = (character.getCharacterFont()
						/ getFontCharHorizontalSize()) * getFontHeight();
				sourceRect.width = getFontWidth();
				sourceRect.height = getFontHeight();
			}

			// Update the character
			setCharacterInBufferAt(x + getWidth() * y, character);
		}
	}

	// Draw the content rendered in the current frame inside texture.
	screen.display();

	// Get the reference to the contet and draw in the window.
	sf::Sprite sprite(screen.getTexture());
	window.draw(sprite);

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
	if (!bitmap.loadFromFile(getFontfile()))
	{
		// Throw Exception
		throw std::runtime_error("The type of image cannot will be opened.\n");
	}

	const auto [width, height] = bitmap.getSize();
	setFontWidth(width / getFontCharHorizontalSize());
	setFontHeight(height / getFontCharVerticalSize());

	// Satisfies post-condition
	// allocated bool array for colored flags
	createTablesOfCharacters();

	screen.create(getWidth() * getFontWidth(), getHeight() * getFontHeight());
	// Update the size of window with the new dimension of font
	window.create(sf::VideoMode(getWidth() * getFontWidth(),
			getHeight() * getFontHeight()), "SFML Renderer");
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
