// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE("Verify that the console constructed for default is initialized with 80 of width and 50 of height")
{
	Console console {};

	CHECK(console.getWidth() == 80);
	CHECK(console.getHeight() == 50);
}