// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE("Verify the construction of character by default")
{
	Char aChar {};

	CHECK(aChar.getCharacter() == ' ');
	CHECK(aChar.getBackground().equals({0, 0, 0}));
	CHECK(aChar.getForeground().equals({255, 255, 255}));
}