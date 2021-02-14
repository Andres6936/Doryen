// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <string>
#include <ostream>
#include <iostream>
#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE("Verify the construction for image class with parameters")
{
	// Is important initialize the Console class, because this initialize the
	// renderer to use, if not is initialized the renderer the class Image not
	// work.
	Console console {10, 10};

	// Here begin the test
	Image image {69, 36};

	CHECK(image.getWidth() == 69);
	CHECK(image.getHeight() == 36);
	CHECK(image.getSize().equals({69, 36}));
}

TEST_CASE("Verify the construction of an empty image")
{
// Is important initialize the Console class, because this initialize the
	// renderer to use, if not is initialized the renderer the class Image not
	// work.
	Console console {10, 10};

	// Here begin the test
	Image image {0, 0};

	CHECK(image.getWidth() == 0);
	CHECK(image.getHeight() == 0);
	CHECK(image.getSize().equals({0, 0}));
}