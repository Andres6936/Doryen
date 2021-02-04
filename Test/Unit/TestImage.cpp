// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE("Default construct for image class")
{
	// Is important initialize the Console class, because this initialize the
	// renderer to use, if not is initialized the renderer the class Image not
	// work.
	Console console {10, 10};

	// Here begin the test
	Image image {50, 50};
}