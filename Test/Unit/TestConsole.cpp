// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <ostream>
#include <iostream>
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
}

TEST_CASE("Verify the width and height of console when it is set with construct parameters")
{
	Console console {250, 100};

	CHECK(console.getWidth() == 250);
	CHECK(console.getHeight() == 100);

	// Internally, the representation of space 2d is realized in one-dimension space (array)
	std::vector<Char>& mainBuffer = accessPrivateBuffer(console);
	// Verify the size correct of buffer
	CHECK(mainBuffer.size() == 250 * 100);
	CHECK(verifyBufferPerDefect(mainBuffer));
}

TEST_CASE("Verify the behavior of write characters out of range. (Do not throw exceptions).")
{
	Console console {};

	try
	{
		// This should not write absolutely nothing to the buffer
		console.writeChar(-1, -1, '@');
		console.writeChar(-1, -1, '@', {69, 36, 69}, {25, 45, 76});
		console.writeChar(-1, -1, '@', {75, 87, 48}, {45, 98, 36}, BlendModes::ADDA);
	}
	// Catch all the exceptions possibles
	catch (...)
	{
		FAIL("An exception has been thrown");
	}

	// Verify that the main buffer and the old buffer is still intact

	// Internally, the representation of space 2d is realized in one-dimension space (array)
	std::vector<Char>& mainBuffer = accessPrivateBuffer(console);
	CHECK(verifyBufferPerDefect(mainBuffer));
}

TEST_CASE("Verify the behavior of write characters out of range. (Do not throw exceptions).")
{
	Console console {};

	try
	{
		// This should not write absolutely nothing to the buffer
		console.writeString(-1, -1, "This is method for draw");
	}
		// Catch all the exceptions possibles
	catch (...)
	{
		FAIL("An exception has been thrown");
	}

	// Verify that the main buffer and the old buffer is still intact

	// Internally, the representation of space 2d is realized in one-dimension space (array)
	std::vector<Char>& mainBuffer = accessPrivateBuffer(console);
	CHECK(verifyBufferPerDefect(mainBuffer));
}