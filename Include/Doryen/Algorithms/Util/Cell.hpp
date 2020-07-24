#ifndef LIBTCOD_CELL_HPP
#define LIBTCOD_CELL_HPP

namespace Doryen
{
    class Cell
	{

	public:

		bool transparent = false;
		bool walkable = false;
		bool fov = false;

		Cell() noexcept = default;

		Cell(bool isTransparent, bool isWalkable, bool isFOV) noexcept;
	};
}

#endif //LIBTCOD_CELL_HPP
