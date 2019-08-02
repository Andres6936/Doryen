#include "FOV/CircularRaycasting.hpp"

#include <algorithm>
#include <bresenham.hpp>

void Doryen::CircularRaycasting::operator()( Doryen::Map &map, int playerX,
                                             int playerY, int maxRadius, bool ligthWalls )
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

void Doryen::CircularRaycasting::castRay( Doryen::Map &map, int xo, int yo,
                                          int xd, int yd, int radiusDouble, bool ligthWalls )
{
    int curX = xo;
    int curY = yo;

    bool in = false;
    bool blocked = false;
    bool end = false;

    Doryen::Line line = Doryen::Line( );
    line.init( xo, yo, xd, yd );

    int offset = curX + map.width * curY;

    if ( offset >= 0 && offset < map.nbcells )
    {
        in = true;
        map.cells[ offset ].fov = true;
    }

    while ( !end )
    {
        end = line.step( &curX, &curY );

        offset = curX + map.width * curY;

        if ( radiusDouble > 0 )
        {
            int currentRadius = ( curX - xo ) * ( curX - xo ) + ( curY - yo ) * ( curY - yo );

            if ( currentRadius > radiusDouble )
            {
                return;
            }
        }

        if ( offset >= 0 && offset < map.nbcells )
        {
            in = true;

            if ( !blocked && !map.cells[ offset ].transparent )
            {
                blocked = true;
            }
            else if ( blocked )
            {
                return;
            }

            if ( ligthWalls || !blocked )
            {
                map.cells[ offset ].fov = true;
            }
        }
        else if ( in )
        {
            return;
        }
    }
}
