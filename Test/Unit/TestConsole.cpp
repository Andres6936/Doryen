// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Hack/AccessPrivateConsole.hpp"

using namespace Doryen;
using namespace Doryen::Test;

TEST_CASE("Verify that the console constructed for default is initialized with 80 of width and 50 of height")
{
	Console console {};

	CHECK(console.getWidth() == 80);
	CHECK(console.getHeight() == 50);

	// Internally, the representation of space 2d is realized in one-dimension space (array)
	std::vector<Char>& mainBuffer = accessPrivateBuffer(console);
	// Verify the size correct of buffer
	CHECK(mainBuffer.size() == 80 * 50);

	// Internally, the representation of space 2d is realized in one-dimension space (array)
	std::vector<Char>& oldBuffer = accessPrivateOldBuffer(console);
	// Verify the size correct of old buffer
	CHECK(oldBuffer.size() == 80 * 50);
}

