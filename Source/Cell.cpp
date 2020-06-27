#include "Doryen/Cell.hpp"

Doryen::Cell::Cell( )
{
    transparent = false;
    walkable = false;
    fov = false;
}

Doryen::Cell::Cell( bool isTransparent, bool isWalkable, bool isFOV )
{
    transparent = isTransparent;
    walkable = isWalkable;
    fov = isFOV;
}
