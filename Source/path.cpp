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



// ----------------- //
// Dijkstra          //
// written by Mingos //
// ----------------- //

#include <path.hpp>

//ctor
TCODDijkstra::TCODDijkstra( Doryen::Map *map, float diagonalCost )
{
    //data = TCOD_dijkstra_new( map->data, diagonalCost );
}

//dtor
TCODDijkstra::~TCODDijkstra( void )
{
    //TCOD_dijkstra_delete( data );
}

//compute distances grid
void TCODDijkstra::compute( int rootX, int rootY )
{
    //TCOD_dijkstra_compute( data, rootX, rootY );
}

//retrieve distance to a given cell
float TCODDijkstra::getDistance( int x, int y )
{
    //return TCOD_dijkstra_get_distance( data, x, y );
}

//create a path
bool TCODDijkstra::setPath( int toX, int toY )
{
    //return ( TCOD_dijkstra_path_set( data, toX, toY ) != 0 );
}

void TCODDijkstra::reverse( )
{
    //TCOD_dijkstra_reverse( data );
}

//walk a path
bool TCODDijkstra::walk( int *x, int *y )
{
    //return TCOD_dijkstra_path_walk( data, x, y ) != 0;
}

bool TCODDijkstra::isEmpty( ) const
{
    //return TCOD_dijkstra_is_empty( data ) != 0;
}

int TCODDijkstra::size( ) const
{
    // return TCOD_dijkstra_size( data );
}

void TCODDijkstra::get( int index, int *x, int *y ) const
{
    // return TCOD_dijkstra_get( data, index, x, y );
}
