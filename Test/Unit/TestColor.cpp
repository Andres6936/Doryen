// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE ("Parser color from hexadecimal string literal (Null terminated)")
{
	Color c = Color::fromString("#192856");

			CHECK(c.equals({ 25, 40, 86 }));
}

TEST_CASE ("Parser color from hexadecimal string variable (Not null terminated)")
{
	std::string hexadecimal = "#192856";
	Color c = Color::fromString(hexadecimal);

			CHECK(c.equals({ 25, 40, 86 }));
}

TEST_CASE ("The lerping with coefficient 0.0f return the start point (color a)")
{
	Color c = Color::lerp({ 126, 126, 126 }, { 33, 33, 33 }, 0.0f);

			CHECK(c.equals({ 126, 126, 126 }));
}