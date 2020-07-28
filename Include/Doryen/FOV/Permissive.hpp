#ifndef LIBTCOD_PERMISSIVE_HPP
#define LIBTCOD_PERMISSIVE_HPP

#include <list>
#include <memory>
#include <vector>
#include <cstdint>

#include "Doryen/Geometry/Point2D.hpp"
#include <Doryen/FOV/Fov.hpp>

namespace Doryen
{

	class Permissive
	{

	private:

		// Definitions (Alias)

		template<typename T>
		using LinkedList = std::list<T>;

		template<typename T>
		using LinkedListNode = typename std::list<T>::iterator;

		// Auxiliary Classes

		class Offset final : public Geometry::Point2D<std::int16_t>
		{

		public:

			Offset() noexcept = default;

			Offset(std::int16_t x, std::int16_t y) noexcept: Geometry::Point2D<std::int16_t>(x, y)
			{
			};

		};

		class Bump final
		{

		public:

			Offset position{};

			std::shared_ptr<Bump> parent{ nullptr };

		};

		class Line final
		{

		public:

			Offset near{};
			Offset far{};

			Line() = default;

			Line(const Offset& _near, const Offset& _far) : near(_near), far(_far)
			{
			};

			// Methods

			bool isBelow(const Offset& point)
			{
				return relativeSlope(point) > 0;
			}

			bool isBelowOrContains(const Offset& point)
			{
				return relativeSlope(point) >= 0;
			}

			bool isAbove(const Offset& point)
			{
				return relativeSlope(point) < 0;
			}

			bool isAboveOrContains(const Offset& point)
			{
				return relativeSlope(point) <= 0;
			}

			bool doesContain(const Offset& point)
			{
				return relativeSlope(point) == 0;
			}

			/**
			 * @return negative if the line is above the point. <br>
			 *  positive if the line is below the point. <br>
			 *  0 if the line is on the point.
			 */
			std::int32_t relativeSlope(const Offset& point)
			{
				return (far.y - near.y) * (far.x - point.x) - (far.y - point.y) * (far.x - near.x);
			}

		};

		class Field final
		{

		public:

			Bump steepBump{};
			Bump shallowBump{};

			Line steep{};
			Line shallow{};

		};

		// References

		Map& map;

		// Variables

		Offset source{};
		Offset quadrant{};

		int rangeLimit{ -1 };

		// Methods

		void computeQuadrant();

		bool actIsBlocked(const Offset& pos);

		/**
		 * <b>Original Implementation:</b>
		 *
		 * A function that takes the X and Y coordinate of a point where X >= 0, Y >= 0,
		 * and X >= Y, and returns the distance from the point to the origin (0,0).
		 *
		 * <b>This implementation:</b>
		 *
		 * The conditions cannot be met.
		 *
		 * Deleted invariant: X >= Y <br>
		 * Modify invariant: X >= 0 to if X >= 0 then X = 0 <br>
		 * Modify invariant: Y >= 0 to if Y >= 0 then Y = 0 <br>
		 *
		 * @param a Coordinate x.
		 * @param b Coordinate y.
		 * @return The distance of two points.
		 */
		std::int32_t getDistance(const std::int32_t a, const std::int32_t b);

	public:

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls, int fovType);
	};

}

#endif //LIBTCOD_PERMISSIVE_HPP
