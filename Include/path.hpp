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

#include <vector>
#include <algorithm>

#include "Enum/Direction.hpp"
#include "Algorithms/Util/Node.hpp"
#include "Algorithms/Enum/SearchState.hpp"

class TCODLIB_API ITCODPathCallback
{
public :
    virtual ~ITCODPathCallback( )
    { }

    virtual float getWalkCost( int xFrom, int yFrom,
            int xTo, int yTo, void *userData ) const = 0;
};

// For sorting the heap the STL needs compare function that
// lets us compare the f value of two nodes
class HeapCompare
{

public:

    bool operator()( const Doryen::Algorithms::Node *x,
                     const Doryen::Algorithms::Node *y ) const
    {
        return x->f > y->f;
    }

};

/**
 * This toolkit allows to easily calculate the optimal path between two points in
 * your dungeon by using either the <a href="http://en.wikipedia.org/wiki/A*">A*
 * algorithm</a> or <a href="http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm">
 * Dijkstra's algorithm</a>.
 *
 * @note Please note that the paths generated with the two algorithms may differ
 * slightly. Due to how they're implemented, A* will usually prefer diagonal moves
 * over orthogonal, while Dijkstra will have the opposite preference. In other
 * words, paths from point X to point Y will look like this:
 *
 * @code
 * Dijkstra:      A*:
 * ..........   ..........
 * .X........   .X*.......
 * ..*.......   ...**.....
 * ...*......   .....**...
 * ....****Y.   .......*Y.
 * ..........   ..........
 * @endcode
 */
class TCODLIB_API TCODPath
{

protected :

    TCOD_path_t data;

private:

    /**
     * Coordinates of the creature position.
     */
    int ox;

    /**
     * Coordinates of the creature position.
     */
    int oy;

    /**
     * Coordinates of the creature's destination.
     */
    int dy;

    /**
     * Coordinates of the creature's destination.
     */
    int dx;

    /**
     * Map size.
     */
    int w;

    /**
     * Map size.
     */
    int h;

    /**
     * Cost of a diagonal movement compared to an horizontal
     * or vertical movement. On a standard cartesian map, it
     * should be sqrt(2) (1.41f).
     */
    float diagonalCost;

    /**
     * wxh djikstra distance grid (covered distance).
     */
    std::vector <float> grid;

    /**
     * wxh A* score grid (covered distance + estimated remaining distance).
     */
    std::vector <float> heur;

    /**
     * heap used in the algorithm. stores the offset in grid/heur (offset=x+y*w).
     */
    std::vector <unsigned long> heap;

    /**
     * wxh 'previous' grid : direction to the previous cell.
     */
    std::vector <Doryen::Direction> prev;

    /**
     * List of Direction to follow the path.
     */
    std::vector <Doryen::Direction> path;

    TCOD_path_func_t func;

    /**
     * Copy of the map.
     */
    Doryen::Map map;

    std::vector<Doryen::Algorithms::Node *> openList;

    std::vector<Doryen::Algorithms::Node *> closedList;

    std::vector<Doryen::Algorithms::Node *> successors;

    SearchState state = SearchState::NOT_INITIALISED;

    int steps = 0;

    Doryen::Algorithms::Node *start;

    Doryen::Algorithms::Node *goal;

    Doryen::Algorithms::Node *currentSolutionNode;

    bool cancelRequest = false;

public :

    /**
     * @brief Allocating a pathfinder from a map.
     *
     * First, you have to allocate a path using a map. {Doryen::Map}.
     *
     * @param map The map. The path finder will use the 'walkable' property of
     * the cells to find a path.
     *
     * @param diagonalCost Cost of a diagonal movement compared to an horizontal
     * or vertical movement. On a standard cartesian map, it should be sqrt(2) (1.41f).
     *
     * @note It you want the same cost for all movements, use 1.0f.
     * @note If you don't want the path finder to use diagonal movements, use 0.0f.
     */
    explicit TCODPath( const Doryen::Map &map, float diagonalCost = 1.41f );

    /**
     * @brief Destroying a path.
     *
     * To release the resources used by a path, destroy it.
     */
    virtual ~TCODPath( );

    /**
     * @brief Computing an A* path.
     *
     * Once you created a TCODPath object, you can compute the path between two points.
     *
     * @param originX Coordinates of the origin of the path.
     * @param originY Coordinates of the origin of the path.
     * @param destinationX Coordinates of the destination of the path.
     * @param destinationY Coordinates of the destination of the path.
     *
     * @note Both points {dx and dy} should be inside the map, and at a walkable position.
     * @note The function returns false if there is no possible path.
     *
     * @return True if there is posible path, false otherwise.
     */
    bool compute( int originX, int originY, int destinationX, int destinationY );

    /**
     * @brief Reversing a path.
     *
     * Once you computed a path, you can exchange origin and destination.
     */
    void reverse( );

    /**
     * Once the path has been computed, you can get information about it using
     * of one those functions.
     *
     * You can read the current origin and destination cells with getOrigin/getDestination.
     *
     * @note Note that when you walk the path, the origin changes at each step.
     *
     * @param x The function returns the cell coordinates in these variables.
     * @param y The function returns the cell coordinates in these variables.
     */
    void getOrigin( int *x, int *y ) const;

    /**
     * Once the path has been computed, you can get information about it using
     * of one those functions.
     *
     * You can read the current origin and destination cells with getOrigin/getDestination.
     *
     * @param x The function returns the cell coordinates in these variables.
     * @param y The function returns the cell coordinates in these variables.
     */
    void getDestination( int *x, int *y ) const;

    /**
     * @brief Getting the path length.
     *
     * You can get the number of steps needed to reach destination.
     *
     * @return number of steps needed to reach destination.
     */
    int size( ) const;

    /**
     * @brief Read the path cells' coordinates.
     *
     * You can get the coordinates of each point along the path.
     *
     * @param index Step number. 0 <= index < path size.
     * @param x Address of the variables receiving the coordinates of the point.
     * @param y Address of the variables receiving the coordinates of the point.
     */
    void get( int index, int *x, int *y );

    /**
     * @brief Checking if the path is empty.
     *
     * If you want a creature to follow the path, a more convenient way is to
     * walk the path.
     *
     * You know when you reached destination when the path is empty.
     *
     * @return True if the path is empty (reached destination), false otherwise.
     */
    bool isEmpty( ) const;

    /**
     * @brief Walking the path.
     *
     * You can walk the path and go to the next step.
     *
     * @note Note that walking the path consume one step (and decrease
     * the path size by one).
     *
     * The function returns false if recalculateWhenNeeded is false and
     * the next cell on the path is no longer walkable, or
     * if recalculateWhenNeeded is true, the next cell on the path is
     * no longer walkable and no other path has been found.
     *
     * @note recalculateWhenNeeded only applies to A*.
     *
     * @param x Address of the variables receiving the coordinates of
     * the next point.
     *
     * @param y Address of the variables receiving the coordinates of
     * the next point.
     *
     * @param recalculateWhenNeeded If the next point is no longer
     * walkable (another creature may be in the way), recalculate
     * a new path and walk it.
     *
     * @return false if recalculateWhenNeeded is false and the next
     * cell on the path is no longer walkable, or if recalculateWhenNeeded
     * is true, the next cell on the path is no longer walkable and
     * no other path has been found.
     */
    bool walk( int *x, int *y, bool recalculateWhenNeeded );
};

//Dijkstra kit
class TCODLIB_API TCODDijkstra
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

private:
    TCOD_dijkstra_t data;
    struct WrapperData
    {
        void *userData;
        const ITCODPathCallback *listener;
    } cppData;
};

#endif

