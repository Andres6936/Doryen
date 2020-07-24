// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include "Doryen/libtcod.hpp"

using namespace Doryen;

TEST_CASE ("Overflow for operations of multiply")
{
	// Element Null
	Color white{ 0, 0, 0 };
	white.multiply({ 255, 255, 255 });

			CHECK(white.equals({ 0, 0, 0 }));

	// Element Neutral
	Color neutral{ 1, 1, 1 };
	neutral.multiply({ 255, 255, 255 });

			CHECK(neutral.equals({ 255, 255, 255 }));

	// Element Total
	Color black{ 255, 255, 255 };
	black.multiply({ 255, 255, 255 });

			CHECK(black.equals({ 255, 255, 255 }));

	// Element Component Red
	Color red{ 255, 0, 0 };
	red.multiply({ 255, 255, 255 });

			CHECK(red.equals({ 255, 0, 0 }));

	// Element Component Green
	Color green{ 0, 255, 0 };
	green.multiply({ 255, 255, 255 });

			CHECK(green.equals({ 0, 255, 0 }));

	// Element Component Blue
	Color blue{ 0, 0, 255 };
	blue.multiply({ 255, 255, 255 });

			CHECK(blue.equals({ 0, 0, 255 }));
}