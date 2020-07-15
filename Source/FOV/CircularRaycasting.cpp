#include <algorithm>

#include <Doryen/Algorithms/Drawing/Bresenham.hpp>
#include "Doryen/FOV/CircularRaycasting.hpp"

void Doryen::CircularRaycasting::operator()(Doryen::Map& map, int playerX,
		int playerY, int maxRadius, bool ligthWalls)
{
	int xMin = 0;
	int yMin = 0;

	int xMax = map.width;
	int yMax = map.height;

    int radiusDouble = maxRadius * maxRadius;

	if (maxRadius > 0)
	{
		xMin = std::max(0, playerX - maxRadius);
		yMin = std::max(0, playerY - maxRadius);

		xMax = std::min(map.width, playerX + maxRadius + 1);
		yMax = std::min(map.height, playerY + maxRadius + 1);
	}

	for (Cell& cell : map.cells)
	{
		cell.fov = false;
	}

	int xo = xMin;
	int yo = yMin;

	while (xo < xMax)
	{
		castRay(map, playerX, playerY, xo++, yo, radiusDouble, ligthWalls);
	}

	xo = xMax - 1;
    yo = yMin + 1;

    while ( yo < yMax )
    {
        castRay( map, playerX, playerY, xo, yo++, radiusDouble, ligthWalls );
    }

    xo = xMax - 2;
    yo = yMax - 1;

    while ( xo >= 0 )
    {
        castRay( map, playerX, playerY, xo--, yo, radiusDouble, ligthWalls );
    }

    xo = xMin;
    yo = yMax - 2;

    while ( yo > 0 )
    {
        castRay( map, playerX, playerY, xo, yo--, radiusDouble, ligthWalls );
    }

    if ( ligthWalls )
    {
        postProcessing( map, xMin, yMin, playerX, playerY, -1, -1 );
        postProcessing( map, playerX, yMin, xMax - 1, playerY, 1, -1 );
        postProcessing( map, xMin, playerY, playerX, yMax - 1, -1, 1 );
        postProcessing( map, playerX, playerY, xMax - 1, yMax - 1, 1, 1 );
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

	Algorithms::Line line = Algorithms::Line();
    line.init( xo, yo, xd, yd );

    int offset = curX + map.width * curY;

	if (offset >= 0 && offset < map.cells.size())
	{
		in = true;
		map.cells[offset].fov = true;
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

		if (offset >= 0 && offset < map.cells.size())
		{
			in = true;

			if (!blocked && !map.cells[offset].transparent)
			{
				blocked = true;
			}
			else if (blocked)
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

void Doryen::CircularRaycasting::postProcessing( Doryen::Map &map, int x0,
                                                 int y0, int x1, int y1, int dx, int dy )
{
    for ( int cx = x0; cx <= x1; cx++ )
    {
        for ( int cy = y0; cy <= y1; cy++ )
        {
            int x2 = cx + dx;
            int y2 = cy + dy;

            unsigned int offset = cx + map.width * cy;

			if (offset < map.cells.size() && map.cells[offset].fov == 1
				&& map.cells[offset].transparent)
			{
				if (x2 >= x0 && x2 <= x1)
				{
					unsigned int offset2 = x2 + map.width * cy;

					if (offset2 < map.cells.size() && !map.cells[offset2].transparent)
					{
						map.cells[offset2].fov = true;
					}
                }

                if ( y2 >= y0 && y2 <= y1 )
                {
                    unsigned int offset2 = cx + map.width * y2;

					if (offset2 < map.cells.size() && !map.cells[offset2].transparent)
					{
						map.cells[offset2].fov = true;
					}
                }

                if ( x2 >= x0 && x2 <= x1 && y2 >= y0 && y2 <= y1 )
                {
                    unsigned int offset2 = x2 + map.width * y2;

					if (offset2 < map.cells.size() && !map.cells[offset2].transparent)
					{
						map.cells[offset2].fov = true;
					}
                }
            }
        }
    }
}
