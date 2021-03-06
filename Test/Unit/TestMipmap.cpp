// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Doryen/Image/Mipmap.hpp"

using namespace Doryen;

TEST_CASE("Size for default is {Width: 0, Height: 0}")
{
	Mipmap mipmap {};

	CHECK(mipmap.isEmpty() == true);
	CHECK(mipmap.getSize().equals({0, 0}));
}