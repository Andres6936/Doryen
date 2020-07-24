#include "Doryen/Algorithms/Util/Cell.hpp"

Doryen::Cell::Cell(bool isTransparent, bool isWalkable, bool isFOV) noexcept
{
    transparent = isTransparent;
    walkable = isWalkable;
    fov = isFOV;
}
