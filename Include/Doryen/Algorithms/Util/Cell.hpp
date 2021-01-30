#ifndef LIBTCOD_CELL_HPP
#define LIBTCOD_CELL_HPP

namespace Doryen
{

	/**
	 * The cell define a point a two-dimensional space, represent the current
	 * state of that point, indicate whether it is traversable, transparent or
	 * may be in the field of view of some entity.
	 */
	class Cell
	{

	public:

		// Properties

		/**
		 * Define if a entity (usually the player) has in its field of view this cell.
		 */
		bool fov = false;

		/**
		 * Define the possibility of traversing this cell for some entity.
		 */
		bool traversable = false;

		/**
		 * Define the property to see other cells through this cell.
		 */
		bool transparent = false;

		// Construct

		Cell() noexcept = default;

		Cell(bool isTransparent, bool isWalkable, bool isFOV) noexcept;

		// Getters

		bool isFov() const;

		bool isTraversable() const;

		bool isTransparent() const;

		// Setters

		void setFov(bool fov);

		void setTraversable(bool traversable);

		void setTransparent(bool transparent);

	};
}

#endif //LIBTCOD_CELL_HPP
