#include <algorithm>
#include "FOV/CircularRaycasting.hpp"

void CircularRaycasting::operator()( Doryen::Map &map, int playerX, int playerY,
                                     int maxRadius, bool ligthWalls )
{
    int xMin = 0;
    int yMin = 0;

    int xMax = map.width;
    int yMax = map.height;

    int radiusDouble = maxRadius * maxRadius;

    if ( maxRadius > 0 )
    {
        xMin = std::max( 0, playerX - maxRadius );
        yMin = std::max( 0, playerY - maxRadius );

        xMax = std::min( map.width, playerX + maxRadius + 1 );
        yMax = std::min( map.height, playerY + maxRadius + 1 );
    }

    for ( int i = 0; map.nbcells; i++ )
    {
        map.cells[ i ].fov = false;
    }

    int xo = xMin;
    int yo = yMin;

    while ( xo < xMax )
    {

    }
}

void CircularRaycasting::castRay( Doryen::Map &map, int xo, int yo, int xd, int yd, int radiusDouble, bool ligthWalls )
{
    int curX = xo;
    int curY = yo;
}
