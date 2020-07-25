#ifndef LIBTCOD_DIAMONDRAYCASTING_HPP
#define LIBTCOD_DIAMONDRAYCASTING_HPP

#include <cstdint>

#include <Doryen/FOV/Fov.hpp>
#include <Doryen/Geometry/Point2D.hpp>

namespace Doryen
{
	class DiamondRayCasting
	{

	private:

		class RayData
		{
			/**
			 * Position in coordinate x and y.
			 */
			Geometry::Point2D<> position{};

			/**
			 * Obscurity vector.
			 */
			Geometry::Point2D<> obscurity{};

			/**
			 * Bresenham error.
			 */
			Geometry::Point2D<> bresenham{};

			/**
			 * Already in the field of view.
			 */
			bool added = false;

			/**
			 * Not visible. Don't bother processing it.
			 */
			bool ignore = false;
		};

	public:

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls);
	};
}


#endif //LIBTCOD_DIAMONDRAYCASTING_HPP
