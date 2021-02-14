// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <string>
#include <ostream>
#include <iostream>
#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE ("Equality of size")
{
	Geometry::Size size1{ 5, 8 };
	Geometry::Size size2{ 5, 8 };

			CHECK(size1.equals(size2));
}

TEST_CASE ("Not equality of size")
{
	Geometry::Size size1{ 5, 8 };
	Geometry::Size size2{ 8, 5 };

			CHECK(not size1.equals(size2));
}

TEST_CASE("Verify behavior of Copy constructor")
{
	Geometry::Size sizeOriginal {69, 36};
	Geometry::Size sizeCopy {sizeOriginal};

	REQUIRE(sizeCopy.equals(sizeOriginal));

	sizeCopy.setWidth(72);
	sizeCopy.setHeight(87);

	CHECK(sizeOriginal.equals({69, 36}));
	CHECK(sizeCopy.equals({72, 87}));
}

SCENARIO ("Equality and not equality for 2 sizes")
{
			GIVEN("Two sizes with the same width and height")
	{
		Geometry::Size size1{ 5, 8 };
		Geometry::Size size2{ 5, 8 };

				AND_WHEN("The two size not change of dimension")
		{
					THEN("The width and height is the same for the two sizes")
			{
						CHECK(size1.equals(size2));
			}
		}
	}

			GIVEN("Two sizes with different dimensions")
	{
		Geometry::Size size1{ 5, 8 };
		Geometry::Size size2{ 8, 5 };

				AND_WHEN("The two size not change of dimension")
		{
					THEN("The width and height is different for the two sizes")
			{
						CHECK(not size1.equals(size2));
			}
		}
	}

			GIVEN("Two sizes with the same dimension")
	{
		Geometry::Size size1{ 5, 8 };
		Geometry::Size size2{ 5, 8 };

				AND_WHEN("It change of width or height")
		{
			size1.setWidth(12);

					THEN("The two size not are same")
			{
						CHECK(not size1.equals(size2));
			}
		}
	}

			GIVEN("Two sizes with different dimensions")
	{
		Geometry::Size size1{ 5, 8 };
		Geometry::Size size2{ 8, 5 };

				AND_WHEN("The two size change to the same width and height")
		{
			size1.setWidth(5);
			size1.setHeight(7);

			size2.setWidth(5);
			size2.setHeight(7);

					THEN("The width and height is same for the two sizes")
			{
						CHECK(size1.equals(size2));
			}
		}
	}
}