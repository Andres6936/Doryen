// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

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
