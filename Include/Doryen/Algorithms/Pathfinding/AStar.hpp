#ifndef LIBTCOD_ASTAR_HPP
#define LIBTCOD_ASTAR_HPP

#include <vector>
#include <deque>
#include <algorithm>

#include "Doryen/Geometry/Point2D.hpp"
#include "Doryen/Algorithms/Util/Node.hpp"
#include "Doryen/Algorithms/Enum/SearchState.hpp"

// For sorting the heap the STL needs compare function that
// lets us compare the f value of two nodes
class HeapCompare
{

public:

	bool operator()(const Doryen::Algorithms::Node* x,
			const Doryen::Algorithms::Node* y) const
    {
        return x->f > y->f;
    }

};

namespace Doryen
{
    namespace Algorithms
    {
        namespace Pathfinding
        {

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
            class AStar
            {

            private:

                /**
                 * Copy of the map.
                 */
                Doryen::Map map;

                std::deque<Doryen::Geometry::Point2D<>> pointList;

                std::vector <Doryen::Algorithms::Node *> openList;

                std::vector <Doryen::Algorithms::Node *> closedList;

                std::vector <Doryen::Algorithms::Node *> successors;

                SearchState state = SearchState::NOT_INITIALISED;

                int steps = 0;

                Doryen::Algorithms::Node *start = nullptr;

                Doryen::Algorithms::Node *goal = nullptr;

                Doryen::Algorithms::Node *currentSolutionNode = nullptr;

                /**
                 * This method is called on two occasions:
                 * 1. When a search fails.
                 * 2. You want to free all the memory used.
                 */
                void freeAllNodes( );

                /*
                 * This call is made by the search class
                 * when the search ends. A lot of nodes
                 * may be created that are still present
                 * when the search ends. They will be
                 * deleted by this routine once the
                 * search ends.
                 */
				void freeUnusedNodes();

			public :

				/**
				 * Allow the instance of a object without needed of specify
				 * a initial map.
				 *
				 * @note The use of AStar without a map associate cause the
				 *  throw of exceptions.
				 */
				AStar() = default;

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
                explicit AStar( const Doryen::Map &map );

                /**
                 * @brief Destroying a path.
                 *
                 * To release the resources used by a path, destroy it.
                 */
                virtual ~AStar( );

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
                void compute( int originX, int originY, int destinationX, int destinationY );

                /**
                 * @brief Free the solution nodes.
                 *
                 * This is done to clean up all used Node memory when you are done with the search.
                 */
                void freeSolutionNodes( );

                /**
                 * This method should be called, after of call to method Compute and before of
                 * call to any other method.
                 *
                 * @return True if the algorithm find an route to goal, false otherwise.
                 */
                bool findPath( );

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
				Doryen::Geometry::Point2D<> getOriginPoint2D() const;

				/**
				 * Once the path has been computed, you can get information about it using
				 * of one those functions.
				 *
				 * You can read the current origin and destination cells with getOrigin/getDestination.
				 *
				 * @param x The function returns the cell coordinates in these variables.
				 * @param y The function returns the cell coordinates in these variables.
				 */
				Doryen::Geometry::Point2D<> getDestinationPoint2D() const;

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
				Doryen::Geometry::Point2D<> getPoint2DAt(int index);

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
				Doryen::Geometry::Point2D<> walk();
            };
        }
    }
}

#endif //LIBTCOD_ASTAR_HPP
