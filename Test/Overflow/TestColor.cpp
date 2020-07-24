// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Doryen/libtcod.hpp"

using namespace Doryen;

TEST_CASE ("Overflow for operations of multiply for another color")
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

TEST_CASE ("Overflow for operations of multiply for a scalar positive")
{
	// Element Null
	Color white{ 0, 0, 0 };
	white.multiply(255.0f);

			CHECK(white.equals({ 0, 0, 0 }));

	// Element Neutral
	Color neutral{ 1, 1, 1 };
	neutral.multiply(255.0f);

			CHECK(neutral.equals({ 255, 255, 255 }));

	// Element Total
	Color black{ 255, 255, 255 };
	black.multiply(255.0f);

			CHECK(black.equals({ 255, 255, 255 }));

	// Element Component Red
	Color red{ 255, 0, 0 };
	red.multiply(255.0f);

			CHECK(red.equals({ 255, 0, 0 }));

	// Element Component Green
	Color green{ 0, 255, 0 };
	green.multiply(255.0f);

			CHECK(green.equals({ 0, 255, 0 }));

	// Element Component Blue
	Color blue{ 0, 0, 255 };
	blue.multiply(255.0f);

			CHECK(blue.equals({ 0, 0, 255 }));
}

TEST_CASE ("Overflow for operations of multiply for a scalar negative")
{
	// Element Null
	Color white{ 0, 0, 0 };
	white.multiply(-255.0f);

			CHECK(white.equals({ 0, 0, 0 }));

	// Element Neutral
	Color neutral{ 1, 1, 1 };
	neutral.multiply(-255.0f);

			CHECK(neutral.equals({ 0, 0, 0 }));

	// Element Total
	Color black{ 255, 255, 255 };
	black.multiply(-255.0f);

			CHECK(black.equals({ 0, 0, 0 }));

	// Element Component Red
	Color red{ 255, 0, 0 };
	red.multiply(-255.0f);

			CHECK(red.equals({ 0, 0, 0 }));

	// Element Component Green
	Color green{ 0, 255, 0 };
	green.multiply(-255.0f);

			CHECK(green.equals({ 0, 0, 0 }));

	// Element Component Blue
	Color blue{ 0, 0, 255 };
	blue.multiply(-255.0f);

			CHECK(blue.equals({ 0, 0, 0 }));
}

TEST_CASE ("Overflow for operations of add for another color")
{
	// Element Null
	Color white{ 0, 0, 0 };

	white.add({ 0, 0, 0 });
			CHECK(white.equals({ 0, 0, 0 }));

	white.add({ 255, 255, 255 });
			CHECK(white.equals({ 255, 255, 255 }));

	// Element Neutral
	Color neutral{ 1, 1, 1 };
	neutral.add({ 255, 255, 255 });

			CHECK(neutral.equals({ 255, 255, 255 }));

	// Element Total
	Color black{ 255, 255, 255 };
	black.add({ 255, 255, 255 });

			CHECK(black.equals({ 255, 255, 255 }));

	// Element Component Red
	Color red{ 255, 0, 0 };
	red.add({ 255, 0, 0 });

			CHECK(red.equals({ 255, 0, 0 }));

	// Element Component Green
	Color green{ 0, 255, 0 };
	green.add({ 0, 255, 0 });

			CHECK(green.equals({ 0, 255, 0 }));

	// Element Component Blue
	Color blue{ 0, 0, 255 };
	blue.add({ 0, 0, 255 });

			CHECK(blue.equals({ 0, 0, 255 }));

	Color gray{ 127, 127, 127 };
	gray.add({ 50, 50, 50 });

			CHECK(gray.equals({ 177, 177, 177 }));
}