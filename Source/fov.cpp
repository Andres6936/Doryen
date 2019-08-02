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

#include <FOV/CircularRaycasting.hpp>
#include <fov.hpp>


#include "libtcod.hpp"
#include "fov_types.h"

Doryen::Map::Map( )
{
    width = 0;
    height = 0;
    nbcells = 0;
    cells = nullptr;
}

Doryen::Map::Map( int width, int height )
{
    if ( width < 0 || height < 0 )
    {
        // Throw Error
    }
    else
    {
        this->width = width;
        this->height = height;
        this->nbcells = width * height;

        this->cells = new Doryen::Cell[nbcells]( );
    }
}

void Doryen::Map::clear( bool transparent, bool walkable )
{
    for ( int i = 0; i < width * height; i++ )
    {
        cells[ i ].transparent = transparent;
        cells[ i ].walkable = walkable;
        cells[ i ].fov = false;
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
    // Comparamos el tamaño del mapa original (this) con el
    // objetivo (source), Si ambos tienen un tamaño similar
    // (esto es, this->nbcells == source.nbcells), no hay necesidad
    // de elimnar el mapa (this) para volver
    // a reservar, simplemente sobreescribimos el mapa.

    // En caso de no ser igual el tamaño de los mapas, eliminamos,
    // reservamos y sobreescribimos.

    if ( this->nbcells != source.nbcells )
    {
        // Eliminamos el mapa.
        delete[] this->cells;

        // Reservamos una nueva lista de celdas del mismo tamaño que
        // el source.
        this->cells = new Doryen::Cell[source.nbcells]( );

        // Copiamos las variables miembros del source.
        this->nbcells = source.nbcells;
        this->width = source.width;
        this->height = source.height;
    }
    else
    {
        // Recordemos que en este punto, el tamaño de los mapas {this,
        // como source} son iguales.
        // Restamos 1 porque empezamos a contar desde 0.
        for ( int i = 0; i <= nbcells - 1; i++ )
        {
            this->cells[ i ].transparent = source.cells[ i ].transparent;
            this->cells[ i ].walkable = source.cells[ i ].walkable;
            this->cells[ i ].fov = source.cells[ i ].fov;
        }
    }
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

    }
    else if ( algo == FOV_SHADOW )
    {

    }
    else if ( algo == FOV_PERMISSIVE_0 )
    {

    }
    else if ( algo == FOV_PERMISSIVE_1 )
    {

    }
    else if ( algo == FOV_PERMISSIVE_2 )
    {

    }
    else if ( algo == FOV_PERMISSIVE_3 )
    {

    }
    else if ( algo == FOV_PERMISSIVE_4 )
    {

    }
    else if ( algo == FOV_PERMISSIVE_5 )
    {

    }
    else if ( algo == FOV_PERMISSIVE_6 )
    {

    }
    else if ( algo == FOV_PERMISSIVE_7 )
    {

    }
    else if ( algo == FOV_PERMISSIVE_8 )
    {

    }
    else if ( algo == FOV_RESTRICTIVE )
    {

    }
    else
    {
        // Throw Error
    }
}

bool Doryen::Map::isInFov( int x, int y ) const
{
    return cells[ x + width * y ].fov;
}

bool Doryen::Map::isTransparent( int x, int y ) const
{
    return cells[ x + width * y ].transparent;
}

bool Doryen::Map::isWalkable( int x, int y ) const
{
    return cells[ x + width * y ].walkable;
}

int Doryen::Map::getWidth( ) const
{
    return width;
}

int Doryen::Map::getHeight( ) const
{
    return height;
}

Doryen::Map::~Map( )
{
    delete[] cells;
}
