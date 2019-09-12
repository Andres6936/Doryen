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

#ifndef _TCOD_PATH_HPP
#define _TCOD_PATH_HPP


#include <fov.hpp>

class ITCODPathCallback
{
public :
    virtual ~ITCODPathCallback( )
    { }

    virtual float getWalkCost( int xFrom, int yFrom,
            int xTo, int yTo, void *userData ) const = 0;
};

//Dijkstra kit
class TCODDijkstra
{
public:
    TCODDijkstra( Doryen::Map *map, float diagonalCost = 1.41f );

    ~TCODDijkstra( void );

    /**
    @PageName path_compute
    @FuncTitle Computing a Dijkstra grid
    @FuncDesc In case of Dijkstra, this works in a slightly different way. In order to be able to compute a path, Dijkstra must first analyse the distances from the selected root (origin) node to all other nodes:
    @Cpp void TCODDijkstra::compute(int rootX, int rootY)
    @C void TCOD_dijkstra_compute(TCOD_dijkstra_t dijkstra, int root_x, int root_y)
    @Py dijkstra_compute(dijkstra, root_x, root_y)
    @C# void TCODDijkstra::compute(int rootX, int rootY)
    @Param dijkstra	In the C version, the path handler returned by a creation function.
    @Param root_x,root_y	Coordinates of the root node (origin) of the path.
        The coordinates should be inside the map, at a walkable position. Otherwise, the function's behaviour will be undefined.
    */
    void compute( int rootX, int rootY );

    /**
    @PageName path_compute
    @FuncTitle Computing a path from a Dijkstra grid
    @FuncDesc After the map is analysed and all the distances from the root node are known, an unlimited number of paths can be set, all originating at the root node, using:
        The path setting function will return true if there's a path from the root node to the destination node. Otherwise, it will return false.
    @Cpp bool TCODDijkstra::setPath(int toX, int toY)
    @C bool TCOD_dijkstra_path_set(TCOD_dijkstra_t dijkstra, int to_x, int to_y)
    @Py dijkstra_path_set(dijkstra, to_x, to_y)
    @C# bool TCODDijkstra::setPath(int toX, int toY)
    @Param dijkstra	In the C version, the path handler returned by a creation function.
    @Param to_x,to_y	Coordinates of the destination node of the path.
    @CppEx
        Doryen::TCODMap *myMap = new Doryen::TCODMap(50,50);
        TCODDijkstra *dijkstra = new TCODDijkstra(myMap); // allocate the path
        dijkstra->compute(25,25); // calculate distance from 25,25 to all other nodes
        dijkstra->setPath(5,5); // calculate a path to node 5,5
        dijkstra->setPath(45,45); //calculate another path from the same origin
    @CEx
        TCOD_map_t my_map=TCOD_map_new(50,50);
        TCOD_dijkstra_t dijkstra = TCOD_dijkstra_new(my_map);
        TCOD_dijkstra_compute(dijkstra,25,25);
        TCOD_dijkstra_path_set(dijkstra,5,5);
        TCOD_dijkstra_path_set(dijkstra,45,45);
    @PyEx
        my_map=libtcod.map_new(50,50)
        dijkstra = libtcod.dijkstra_new(my_map)
        libtcod.dijkstra_compute(dijkstra,25,25)
        libtcod.dijkstra_path_set(dijkstra,5,5)
        libtcod.dijkstra_path_set(dijkstra,45,45)
    */
    bool setPath( int toX, int toY );

    /**
    @PageName path_read
    @FuncTitle Getting the distance from a cell to the root node
    @FuncDesc You can get the distance of any set of coordinates from the root node:
        Note that if the coordinates x,y are outside of the map or are a non-walkable position, the function will return -1.0f. This functionality is only available for Dijkstra's algorithm.
    @Cpp float TCODDijkstra::getDistance(int x, int y)
    @C float TCOD_dijkstra_get_distance(TCOD_dijkstra_t dijkstra, int x, int y)
    @Py dijkstra_get_distance(dijkstra, x, y)
    @C# float TCODDijkstra::getDistance(int x, int y)
    @Param dijkstra	In the C version, the path handler returned by a creation function.
    @Param x,y	The coordinates whose distance from the root node are to be checked
    */
    float getDistance( int x, int y );

    bool walk( int *x, int *y );

    bool isEmpty( ) const;

    void reverse( );

    int size( ) const;

    void get( int index, int *x, int *y ) const;

};

#endif

