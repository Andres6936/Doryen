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
#include "libtcod.hpp"

TCODPath::TCODPath( const Doryen::Map &map, float diagonalCost )
{
    // Copiamos el mapa pasado por parámetro.
    this->map.copy( map );
}

TCODPath::~TCODPath( ) = default;


bool TCODPath::compute( int originX, int originY, int destinationX, int destinationY )
{
    // Check that origin and destination are inside the map.
    if ( originX < 0 || originX >= map.width || originY < 0 || originY >= map.height )
    {
        // Throw Error
        state = SearchState ::INVALID;
        return false;
    }
    else
    {
        // Abbreviation of Node
        using Node = Doryen::Algorithms::Node;

        start = new Node(originX, originY);
        goal = new Node(destinationX, destinationY);

        state = SearchState ::SEARCHING;

        start->g = 0;
        start->h = start->goalDistanceEstimate(goal);
        start->f = start->g + start->h;
        start->parent = nullptr;

        openList.push_back(start);

        steps = 0;

        while (state == SearchState::SEARCHING)
        {
            if (openList.empty() || cancelRequest)
            {
                // FreeAllNodes();
                state = SearchState ::FAILED;
                return false;
            }

            steps += 1;

            Node *node = openList.front();
            std::pop_heap(openList.begin(), openList.end(), HeapCompare());
            openList.pop_back();

            if (node->isGoal(goal))
            {
                goal->parent = node->parent;
                goal->g = node->g;

                // A special case is that the start was passed in as the
                // goal state so handle that here.

                // Better: if ( node.isNotStartStateAndGoalState() )
                if ( ! node->isSameState( start ))
                {
                    delete node;

                    // Set the child pointers in each node (except Goal
                    // which has no child).
                    Node *nodeChild = goal;
                    Node *nodeParent = goal->parent;

                    nodeParent->child = nodeChild;

                    nodeChild = nodeParent;
                    nodeParent = nodeParent->parent;

                    while (nodeChild != start)
                    {
                        nodeParent->child = nodeChild;

                        nodeChild = nodeParent;
                        nodeParent = nodeParent->parent;
                    }
                }

                // FreeUnusedNodes();
                state = SearchState ::SUCCEEDED;
                return true;
            }
            else // Not Goal
            {
                successors.clear();

                bool successorsAdded;

                if (node->parent)
                {
                    successorsAdded = node->getSuccessors( successors, map, node->parent);
                }
                else
                {
                    successorsAdded = node->getSuccessors( successors, map, nullptr);
                }

                if (! successorsAdded)
                {
                    for (Node *nodeSuccessor: successors)
                    {
                        delete nodeSuccessor;
                    }

                    successors.clear();

                    delete node;
                    // FreeAllNodes()

                    state = SearchState ::OUT_OF_MEMORY;
                    return false;
                }

                for (Node *nodeSuccessor: successors)
                {
                    float valueGSuccessor = node->g + node->getCost( nodeSuccessor, map );

                    typename std::vector<Node *>::iterator openListResult;

                    for (openListResult = openList.begin(); openListResult != openList.end(); openListResult += 1)
                    {
                        if ((*openListResult)->isSameState(nodeSuccessor))
                        {
                            break;
                        }
                    }

                    if (openListResult != openList.end())
                    {
                        if ((*openListResult)->g <= valueGSuccessor)
                        {
                            delete nodeSuccessor;

                            continue;
                        }
                    }

                    typename std::vector<Node *>::iterator closedListResult;

                    for (closedListResult = closedList.begin(); closedListResult != closedList.end(); closedListResult += 1)
                    {
                        if ((*closedListResult)->isSameState(nodeSuccessor))
                        {
                            break;
                        }
                    }

                    if (closedListResult != closedList.end())
                    {
                        if ((*closedListResult)->g <= valueGSuccessor)
                        {
                            delete nodeSuccessor;

                            continue;
                        }
                    }

                    nodeSuccessor->parent = node;
                    nodeSuccessor->g = valueGSuccessor;
                    nodeSuccessor->h = nodeSuccessor->goalDistanceEstimate(goal);
                    nodeSuccessor->f = nodeSuccessor->g + nodeSuccessor->h;

                    if (closedListResult != closedList.end())
                    {
                        (*closedListResult)->parent = nodeSuccessor->parent;
                        (*closedListResult)->g = nodeSuccessor->g;
                        (*closedListResult)->h = nodeSuccessor->h;
                        (*closedListResult)->f = nodeSuccessor->f;

                        delete nodeSuccessor;

                        openList.push_back((*closedListResult));

                        closedList.erase(closedListResult);

                        std::push_heap(openList.begin(), openList.end(), HeapCompare());
                    }
                    else if (openListResult != openList.end())
                    {
                        (*openListResult)->parent = nodeSuccessor->parent;
                        (*openListResult)->g = nodeSuccessor->g;
                        (*openListResult)->h = nodeSuccessor->h;
                        (*openListResult)->f = nodeSuccessor->f;

                        delete nodeSuccessor;

                        std::make_heap(openList.begin(), openList.end(), HeapCompare());
                    }
                    else
                    {
                        openList.push_back(nodeSuccessor);

                        std::push_heap(openList.begin(), openList.end(), HeapCompare());
                    }
                }

                closedList.push_back(node);
            }
        }

        return true;
    }
}

bool TCODPath::walk(int *x, int *y, bool recalculateWhenNeeded)
{
    if (state != SearchState::SUCCEEDED)
    {
        // Throw Error
        return false;
    }
    else
    {

    }
}

bool TCODPath::isEmpty() const {
	return TCOD_path_is_empty(data) != 0;
}

void TCODPath::reverse() {
	TCOD_path_reverse(data);
}

int TCODPath::size() const {
	return TCOD_path_size(data);
}

void TCODPath::get(int index, int *x, int *y) const {
	return TCOD_path_get(data,index,x,y);
}

void TCODPath::getOrigin(int *x,int *y) const {
	TCOD_path_get_origin(data,x,y);
}

void TCODPath::getDestination(int *x,int *y) const {
	TCOD_path_get_destination(data,x,y);
}

// ----------------- //
// Dijkstra          //
// written by Mingos //
// ----------------- //

//ctor
TCODDijkstra::TCODDijkstra( Doryen::Map *map, float diagonalCost )
{
    data = TCOD_dijkstra_new(map->data,diagonalCost);
}

//dtor
TCODDijkstra::~TCODDijkstra (void) {
    TCOD_dijkstra_delete(data);
}

//compute distances grid
void TCODDijkstra::compute (int rootX, int rootY) {
    TCOD_dijkstra_compute(data,rootX,rootY);
}

//retrieve distance to a given cell
float TCODDijkstra::getDistance (int x, int y) {
    return TCOD_dijkstra_get_distance(data,x,y);
}

//create a path
bool TCODDijkstra::setPath (int toX, int toY) {
    return (TCOD_dijkstra_path_set(data,toX,toY) != 0);
}

void TCODDijkstra::reverse() {
	TCOD_dijkstra_reverse(data);
}

//walk a path
bool TCODDijkstra::walk (int *x, int *y) {
    return TCOD_dijkstra_path_walk(data,x,y) != 0;
}

bool TCODDijkstra::isEmpty() const {
	return TCOD_dijkstra_is_empty(data) != 0;
}

int TCODDijkstra::size() const {
	return TCOD_dijkstra_size(data);
}

void TCODDijkstra::get(int index, int *x, int *y) const {
	return TCOD_dijkstra_get(data,index,x,y);
}

