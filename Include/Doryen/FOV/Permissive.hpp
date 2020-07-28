#ifndef LIBTCOD_PERMISSIVE_HPP
#define LIBTCOD_PERMISSIVE_HPP

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

		// Variables

		Offset source{};
		Offset quadrant{};

		int rangeLimit{ -1 };

		// Methods

		void computeQuadrant();

	public:

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls, int fovType);
	};

}

#endif //LIBTCOD_PERMISSIVE_HPP
