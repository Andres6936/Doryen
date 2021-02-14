// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <ostream>
#include <iostream>
#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE("Verify that all the properties are set to false for the construct for default")
{
	Cell cell {};

	CHECK(cell.isFov() == false);
	CHECK(cell.isTraversable() == false);
	CHECK(cell.isTransparent() == false);
}

TEST_CASE("Verify the properties define with the construct with parameters")
{
	Cell cell {true, true, true};

	CHECK(cell.isFov() == true);
	CHECK(cell.isTraversable() == true);
	CHECK(cell.isTransparent() == true);
}

TEST_CASE("Verify the change of properties set with the setters")
{
	Cell cell {};

	CHECK(cell.isFov() == false);
	CHECK(cell.isTraversable() == false);
	CHECK(cell.isTransparent() == false);

	cell.setFov(true);
	cell.setTraversable(true);
	cell.setTransparent(true);

	CHECK(cell.isFov() == true);
	CHECK(cell.isTraversable() == true);
	CHECK(cell.isTransparent() == true);
}