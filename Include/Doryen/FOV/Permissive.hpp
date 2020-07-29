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

	/**
	 * Permissive field of view (tile-to-tile)
	 *
	 * The permissive field of view algorithm treats a tile as visible if there's an unobstructed
	 * line from any part of the player's tile to any part of the target tile.
	 *
	 * Most implementations of this use an approximation, such as just testing the corners against
	 * each other, which fails in some cases. Exact implementations work in all cases but tend to
	 * be fairly slow.
	 *
	 * I provide an exact implementation (cleaned up and adapted from a demo by Jonathon Duerig).
	 *
	 * The main features of the algorithm are that it's symmetrical and allows peeking very far
	 * around corners, but it's too permissive for my taste, so I haven't made any effort to
	 * optimize the algorithm.
	 *
	 * That said, the algorithm looks and works much better if all creatures have a short sight
	 * radius.
	 *
	 * The permissive field of view algorithm is always symmetrical.
	 *
	 * @note <b>Pros:</b>
	 *
	 *  - Symmetry. <br>
	 *  - No blind corners. <br>
	 *  - Expansive walls. <br>
	 *  - Continuous point visibility.
	 *
	 * @note <b>Cons:</b>
	 *
	 *  - Slow. <br>
	 *  - No expanding pillar shadows. <br>
	 *  - Perhaps too much visibility around corners.
	 *
	 * @attention Implemented from
	 *  <a href="http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html#permissivecode">
	 *  Permissive Code</a>
	 */
	class Permissive
	{

	private:

		// Definitions (Alias), allow consistence with the original code

		template<typename T>
		using LinkedList = std::list<T>;

		/**
		 * Allow the follow concepts:
		 *
		 * - Readable
		 * - Writable
		 * - Iterator
		 * - Bidirectional Iterator
		 */
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

			Bump() = default;

			Bump(const Offset& _position, std::shared_ptr<Bump> _parent)
					: position(_position), parent(_parent)
			{
			};

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

			bool isBelow(const Offset& point) const
			{
				return relativeSlope(point) > 0;
			}

			bool isBelowOrContains(const Offset& point) const
			{
				return relativeSlope(point) >= 0;
			}

			bool isAbove(const Offset& point) const
			{
				return relativeSlope(point) < 0;
			}

			bool isAboveOrContains(const Offset& point) const
			{
				return relativeSlope(point) <= 0;
			}

			bool doesContain(const Offset& point) const
			{
				return relativeSlope(point) == 0;
			}

			/**
			 * @return negative if the line is above the point. <br>
			 *  positive if the line is below the point. <br>
			 *  0 if the line is on the point.
			 */
			std::int32_t relativeSlope(const Offset& point) const
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

		void addSteepBump(const Offset& point, LinkedListNode<Field> currentField,
				std::vector<Bump>& steepBumps);

		void addShallowBump(const Offset& point, LinkedListNode<Field> currentField,
				std::vector<Bump>& shallowBumps);

		LinkedListNode<Field> checkField(const LinkedListNode<Field> currentField,
				LinkedList<Field>& activeFields);

		LinkedListNode<Field> visitSquare(const Offset& dest, LinkedListNode<Field> currentField,
				std::vector<Bump>& steepBumps,
				std::vector<Bump>& shallowBumps,
				LinkedList<Field>& activeFields);

	public:

		Permissive(Map& _map) : map(_map)
		{
		};

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls, int fovType);
	};

}

#endif //LIBTCOD_PERMISSIVE_HPP
