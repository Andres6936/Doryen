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

		/**
		 * Construct a cell with all properties to false, it is, not be
		 * traversable, not be transparent and not is in the field vision
		 * of any entity.
		 */
		Cell() noexcept = default;

		Cell(bool isTransparent, bool isWalkable, bool isFOV) noexcept;

		// Getters

		/**
		 * @return Returns true if this cell is in the vision field of any
		 * entity, false otherwise.
		 */
		bool isFov() const;

		/**
		 * @return Return true if is possible traversing this cell for any entity.
		 */
		bool isTraversable() const;

		/**
		 * @return Returns true if is possible see other cells through this cell.
		 */
		bool isTransparent() const;

		// Setters

		/**
		 * @param isFov Define if this cell is in the vision field of any entity.
		 */
		void setFov(bool isFov);

		/**
		 * @param isTraversable Define if this cell can be traversable for any entity.
		 */
		void setTraversable(bool isTraversable);

		/**
		 * @param isTransparent Define if this cell is transparent for any entity.
		 */
		void setTransparent(bool isTransparent);

	};
}

#endif //LIBTCOD_CELL_HPP
