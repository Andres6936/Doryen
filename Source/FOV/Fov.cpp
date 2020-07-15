/*
* libtcod 1.5.1
* Copyright (c) 2008,2009,2010,2012 Jice & Mingos
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice or Mingos may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE AND MINGOS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE OR MINGOS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Doryen/FOV/CircularRaycasting.hpp>
#include <Doryen/FOV/DiamondRayCasting.hpp>
#include <Doryen/FOV/Permissive.hpp>
#include <Doryen/FOV/Restrictive.hpp>
#include <Doryen/FOV/ShadowCasting.hpp>
#include <Doryen/FOV/Fov.hpp>

#include "Doryen/fov_types.h"

#include <algorithm>

Doryen::Map::Map()
{
	width = 0;
	height = 0;
}

Doryen::Map::Map(int width, int height)
{
	if (width < 0 || height < 0)
    {
        // Throw Error
    }
    else
    {
        this->width = width;
        this->height = height;

		this->cells.resize(width * height);
    }
}

void Doryen::Map::clear( bool transparent, bool walkable )
{
	for (Cell& cell : cells)
	{
		cell.transparent = transparent;
		cell.walkable = walkable;
		cell.fov = false;
	}
}

void Doryen::Map::setProperties( int x, int y, bool isTransparent, bool isWalkable )
{
    if ( x < 0 || x >= width || y < 0 || y >= height )
    {
        // Throw Error
    }
    else
    {
        cells[ x + width * y ].transparent = isTransparent;
        cells[ x + width * y ].walkable = isWalkable;
    }
}

void Doryen::Map::copy( const Map &source )
{
	this->cells.clear();

	std::copy(source.cells.begin(), source.cells.end(), std::back_inserter(this->cells));

	this->width = source.width;
	this->height = source.height;
}

void Doryen::Map::computeFov( int x, int y, int maxRadius, bool light_walls,
                              TCOD_fov_algorithm_t algo )
{
    // TCOD_map_compute_fov( data, x, y, maxRadius, light_walls, algo );

    if ( algo == FOV_BASIC )
    {
        Doryen::CircularRaycasting circularRaycasting;

        circularRaycasting( *this, x, y, maxRadius, light_walls );
    }
    else if ( algo == FOV_DIAMOND )
	{
		Doryen::DiamondRayCasting diamondRayCasting;

		diamondRayCasting(*this, x, y, maxRadius, light_walls);
	}
    else if ( algo == FOV_SHADOW )
	{
		Doryen::ShadowRayCasting shadowRayCasting;

		shadowRayCasting(*this, x, y, maxRadius, light_walls);
	}
    else if ( algo == FOV_PERMISSIVE_0 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 0);
	}
    else if ( algo == FOV_PERMISSIVE_1 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 1);
	}
    else if ( algo == FOV_PERMISSIVE_2 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 2);
	}
    else if ( algo == FOV_PERMISSIVE_3 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 3);
	}
    else if ( algo == FOV_PERMISSIVE_4 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 4);
	}
    else if ( algo == FOV_PERMISSIVE_5 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 5);
	}
    else if ( algo == FOV_PERMISSIVE_6 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 6);
	}
    else if ( algo == FOV_PERMISSIVE_7 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 7);
	}
    else if ( algo == FOV_PERMISSIVE_8 )
	{
		Doryen::Permissive permissive;

		permissive(*this, x, y, maxRadius, light_walls, 8);
	}
    else if ( algo == FOV_RESTRICTIVE )
	{
		Doryen::Restrictive restrictive;

		restrictive(*this, x, y, maxRadius, light_walls);
	}
    else
    {
        // Throw Error
    }
}

bool Doryen::Map::isInFov( int x, int y ) const
{
    if (x < width || x >= width || y < height || y >= height)
    {
        return cells[ x + width * y ].fov;
    }

    return false;
}

bool Doryen::Map::isTransparent( int x, int y ) const
{
    if (x < width || x >= width || y < height || y >= height)
    {
        return cells[ x + width * y ].transparent;
    }

    return false;
}

bool Doryen::Map::isWalkable( int x, int y ) const
{
    if (x < width || x >= width || y < height || y >= height)
    {
        return cells[ x + width * y ].walkable;
    }

    return false;
}

int Doryen::Map::getWidth( ) const
{
    return width;
}

int Doryen::Map::getHeight( ) const
{
    return height;
}
