// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Hack/AccessPrivateConsole.hpp"

using namespace Doryen;
using namespace Doryen::Test;

/**
 * Verify if a buffer is constructed by default, it is, all the characters are
 * set to ' ' (space), the foreground color is white (RGB: 255, 255, 255) and the
 * background color is black (RGB: 0, 0, 0).
 *
 * @param buffer The buffer to verify.
 * @return True if the buffer passed by parameter is a buffer constructed by
 * default, false otherwise.
 */
bool verifyBufferPerDefect(const std::vector<Char>& buffer)
{
	for(const auto& _char : buffer)
	{
		// First verification: Is the character ' ' (space)
		if (_char.getCharacter() not_eq ' ')
		{
			return false;
		}
		// Second verification: Is the background color character black (RGB: 0, 0, 0)
		else if (!_char.getBackground().equals({0, 0, 0}))
		{
			return false;
		}
		// Third verification: Is the foreground color character white (RGB: 255, 255, 255)
		else if (!_char.getForeground().equals({255, 255, 255}))
		{
			return false;
		}
	}

	return true;
}

TEST_CASE("Verify that the console constructed for default is initialized with 80 of width and 50 of height")
{
	Console console {};

	CHECK(console.getWidth() == 80);
	CHECK(console.getHeight() == 50);

	// Internally, the representation of space 2d is realized in one-dimension space (array)
	std::vector<Char>& mainBuffer = accessPrivateBuffer(console);
	// Verify the size correct of buffer
	CHECK(mainBuffer.size() == 80 * 50);
	CHECK(verifyBufferPerDefect(mainBuffer));

	// Internally, the representation of space 2d is realized in one-dimension space (array)
	std::vector<Char>& oldBuffer = accessPrivateOldBuffer(console);
	// Verify the size correct of old buffer
	CHECK(oldBuffer.size() == 80 * 50);
	CHECK(verifyBufferPerDefect(oldBuffer));
}

