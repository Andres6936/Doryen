#ifndef LIBTCOD_DIAMONDRAYCASTING_HPP
#define LIBTCOD_DIAMONDRAYCASTING_HPP

#include <cstdint>
#include <functional>

#include <Doryen/FOV/Fov.hpp>
#include <Doryen/Geometry/Point2D.hpp>

namespace Doryen
{

	/**
	 * Diamond walls (point-to-tile or point-to-point).
	 *
	 * @note <b>Pros:</b> Pretty fast. Expanding pillar shadows. Expansive walls. No blind corners.
	 *  Mostly continuous point visibility.
	 *
	 * @note <b>Cons:</b> Beam expands too much through a door. Asymmetrical; a small change fixes
	 *  this but loses expansive walls and causes more visual discontinuities.
	 *
	 * Like shadow casting, the diamond walls algorithm treats a tile as visible if there's an
	 * unobstructed line from the center of the player's tile to any part of the target tile.
	 *
	 * However, for the purpose of occlusion it treats walls as though they were diamonds
	 * (embedded in the tile, the remainder being empty space). This eliminates the thin diagonal
	 * spaces from the standard shadow casting algorithm and allows better peeking around corners,
	 * but it causes a couple problems of its own.
	 *
	 * The main new problem is that it is, in my opinion, a bit too permissive, meaning that it
	 * makes too many tiles visible.
	 *
	 * Nonetheless it seems like an improvement on standard shadow casting. (I chose to implement
	 * it by modifying my shadow casting code.)
	 *
	 * As for efficiency, it is somewhat slower than plain shadow casting, since it needs to do
	 * more work per tile, but it's still reasonably fast.
	 *
	 * Treating walls as diamonds actually makes a lot of sense in roguelikes, although it sounds
	 * ridiculous. The reason is that most roguelikes allow monsters to move between diagonally
	 * adjacent walls, and if the walls were square, their corners would be touching, leaving no
	 * space.
	 *
	 * This is illustrated in the picture below. Making the physics of the world more
	 * consistent naturally leads to more intuitive lighting. Diamond walls also allow better
	 * vision around corners, which is usually desirable.
	 *
	 * @image <img src="http://www.adammil.net/files/125/diamonds.png"/>
	 * @image <img src="http://www.adammil.net/files/125/squarecorns.png"/>
	 * @image <img src="http://www.adammil.net/files/125/diamcorns.png"/>
	 *
	 * Unfortunately, there is a theoretical problem with diamond walls. The description states
	 * that lines tangent to a wall diamond aren't considered to intersect it and zero-width beams
	 * of light still illuminate tiles. This provides better vision around corners but also allows
	 * the following case where a monster can see through a wall.
	 *
	 * Implementations are expected to disallow seeing through walls as a special case, which
	 * prevents the problem but requires inconsistent physics.
	 *
	 * @attention Implemented from
	 *  <a href="http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html#diamondcode">
	 *  Diamond Code</a>
	 */
	class DiamondRayCasting
	{

	private:

		/**
		 * Represents the slope Y/X as a rational number
		 */
		class Slope final : public Geometry::Point2D<>
		{

		public:

			// Construct

			Slope(const std::int32_t y, const std::int32_t x) : Geometry::Point2D<>(x, y)
			{
			};

			// Methods

			bool greater(const std::uint32_t y, const std::uint32_t x) const
			{
				return this->y * x > this->x * y;
			}

			bool greaterOrEqual(const std::uint32_t y, const std::uint32_t x) const
			{
				return this->y * x >= this->x * y;
			}

			bool lessOrEqual(const std::uint32_t y, const std::uint32_t x) const
			{
				return this->y * x <= this->x * y;
			}
		};

		Map& _map;

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

		bool blocksLight(std::int32_t x, std::int32_t y,
				std::uint32_t octant, const Geometry::Point2D<>& _origin);

		/**
		 * @param origin The location of the entity whose field of view will be calculated.
		 * @param rangeLimit The maximum distance from the origin that tiles will be lit.
		 *  If equal to -1, no limit will be applied.
		 */
		void compute(std::uint32_t octant, const Geometry::Point2D<>& origin,
				std::uint32_t rangeLimit, std::uint32_t x,
				Slope top, Slope bottom);

	public:

		/**
		 * @post map has been referenced.
		 *
		 * @param map Reference to map.
		 */
		DiamondRayCasting(Map& map) : _map(map)
		{
		};

		void operator()(int playerX, int playerY,
				int maxRadius, bool ligthWalls);
	};
}


#endif //LIBTCOD_DIAMONDRAYCASTING_HPP
