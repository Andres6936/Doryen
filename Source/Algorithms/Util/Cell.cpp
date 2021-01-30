#include "Doryen/Algorithms/Util/Cell.hpp"

Doryen::Cell::Cell(bool isTransparent, bool isWalkable, bool isFOV) noexcept
{
	transparent = isTransparent;
	traversable = isWalkable;
	fov = isFOV;
}
