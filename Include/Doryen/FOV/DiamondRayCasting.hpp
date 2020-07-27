#ifndef LIBTCOD_DIAMONDRAYCASTING_HPP
#define LIBTCOD_DIAMONDRAYCASTING_HPP

#include <cstdint>
#include <functional>

#include <Doryen/FOV/Fov.hpp>
#include <Doryen/Geometry/Point2D.hpp>

namespace Doryen
{
	class DiamondRayCasting
	{

	private:

		class Slope final : public Geometry::Point2D<>
		{

		public:

			// Construct

			Slope(const std::int32_t x, const std::int32_t y) : Geometry::Point2D<>(x, y)
			{
			};

			// Methods

			bool greater(const std::uint32_t x, const std::uint32_t y) const
			{
				return this->y * x > this->x * y;
			}

			bool greaterOrEqual(const std::uint32_t x, const std::uint32_t y) const
			{
				return this->y * x >= this->x * y;
			}

			bool lessOrEqual(const std::uint32_t x, const std::uint32_t y) const
			{
				return this->y * x <= this->x * y;
			}
		};

		Map& _map;

		std::int32_t getDistance(const std::int32_t a, const std::int32_t b);

		bool blocksLight(std::int32_t x, std::int32_t y,
				std::uint32_t octant, const Geometry::Point2D<>& _origin);

		void compute(std::uint32_t octant, Geometry::Point2D<> origin,
				std::uint32_t rangeLimit, std::uint32_t x,
				Slope top, Slope bottom);

	public:

		DiamondRayCasting(Map& map) : _map(map)
		{
		};

		void operator()(Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls);
	};
}


#endif //LIBTCOD_DIAMONDRAYCASTING_HPP
