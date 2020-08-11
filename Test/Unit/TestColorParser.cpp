// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE ("Parser color from hexadecimal")
{
	Color c = Color::fromString("#192856");

			CHECK(c.equals({ 25, 40, 86 }));
}