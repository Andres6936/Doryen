// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_USE_STD_HEADERS

#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

using namespace Doryen;

TEST_CASE ("Equality of point in 2 dimensions")
{
	Geometry::Point2D<> point1{ 5, 8 };
	Geometry::Point2D<> point2{ 5, 8 };

			CHECK(point1.equals(point2));
}

TEST_CASE ("Not equality of point in 2 dimensions")
{
	Geometry::Point2D<> point1{ 5, 8 };
	Geometry::Point2D<> point2{ 8, 5 };

			CHECK(not point1.equals(point2));
}

SCENARIO ("Equality and not equality for 2 points")
{
			GIVEN("Two points with the same coordinate")
	{
		Geometry::Point2D<> point1{ 5, 8 };
		Geometry::Point2D<> point2{ 5, 8 };

				AND_WHEN("The two point not change of coordinate")
		{
					THEN("The coordinate is the same for the two points")
			{
						CHECK(point1.equals(point2));
			}
		}
	}

			GIVEN("Two points with different coordinate")
	{
		Geometry::Point2D<> point1{ 5, 8 };
		Geometry::Point2D<> point2{ 8, 5 };

				AND_WHEN("The two point not change of coordinate")
		{
					THEN("The coordinate is different for the two points")
			{
						CHECK(not point1.equals(point2));
			}
		}
	}

			GIVEN("Two points with the same coordinate")
	{
		Geometry::Point2D<> point1{ 5, 8 };
		Geometry::Point2D<> point2{ 5, 8 };

				AND_WHEN("It change of coordinate in any space")
		{
			point1.setX(12);

					THEN("The two point not are same")
			{
						CHECK(not point1.equals(point2));
			}
		}
	}

			GIVEN("Two points with different coordinate")
	{
		Geometry::Point2D<> point1{ 5, 8 };
		Geometry::Point2D<> point2{ 8, 5 };

				AND_WHEN("The two point change to the same coordinate")
		{
			point1.setX(5);
			point1.setY(7);

			point2.setX(5);
			point2.setY(7);

					THEN("The coordinate is same for the two points")
			{
						CHECK(point1.equals(point2));
			}
		}
	}
}