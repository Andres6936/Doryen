#include <Algorithms/Pathfinding/AStar.hpp>

#include "Exceptions/IllegalMethodCall.hpp"

Doryen::Algorithms::Pathfinding::AStar::AStar( const Doryen::Map &map )
{
    // Copiamos el mapa pasado por parámetro.
    this->map.copy( map );
}

Doryen::Algorithms::Pathfinding::AStar::~AStar( )
{
    freeAllNodes( );
}

void Doryen::Algorithms::Pathfinding::AStar::compute( int originX, int originY, int destinationX, int destinationY )
{
    // Check that origin and destination are inside the map.
    if ( originX < 0 || originX >= map.width || originY < 0 || originY >= map.height )
    {
        // Throw Error
        state = SearchState::INVALID;
        return;
    }
    else
    {
        // Abbreviation of Node
        using Node = Doryen::Algorithms::Node;

        currentSolutionNode = nullptr;

        start = new Node( originX, originY );
        goal = new Node( destinationX, destinationY );

        state = SearchState::SEARCHING;

        start->g = 0;
        start->h = start->goalDistanceEstimate( goal );
        start->f = start->g + start->h;
        start->parent = nullptr;

        openList.push_back( start );

        steps = 0;

        while ( state == SearchState::SEARCHING )
        {
            if ( openList.empty( ))
            {
                state = SearchState::FAILED;
                freeAllNodes( );
                return;
            }

            steps += 1;

            Node *node = openList.front( );

            std::pop_heap( openList.begin( ), openList.end( ), HeapCompare( ));

            openList.pop_back( );

            if ( node->isGoal( goal ))
            {
                goal->parent = node->parent;
                goal->g = node->g;

                // A special case is that the start was passed in as the
                // goal state so handle that here.

                // Better: if ( node.isNotStartStateAndGoalState() )
                if ( !node->isSameState( start ))
                {
                    delete node;

                    // Set the child pointers in each node (except Goal
                    // which has no child).
                    Node *nodeChild = goal;
                    Node *nodeParent = goal->parent;

                    nodeParent->child = nodeChild;

                    nodeChild = nodeParent;
                    nodeParent = nodeParent->parent;

                    while ( nodeChild != start )
                    {
                        nodeParent->child = nodeChild;

                        nodeChild = nodeParent;
                        nodeParent = nodeParent->parent;
                    }
                }

                state = SearchState::SUCCEEDED;

                currentSolutionNode = start;

                // En este punto, el algoritmo A Star ha hallado una ruta
                // hacia el objetivo, sin embargo y por temas de conveniencia
                // recorreremos cada coordenada de la ruta y almacenaremos las
                // coordenadas en una estructura de datos apropiada destinada
                // a esta tarea.

                // delete nodes that aren't needed for the solution.
                freeUnusedNodes( );
                // Limpiamos la estructura de datos.
                pointList.clear( );

                while ( true )
                {
                    Doryen::Geometry::Point2D point(currentSolutionNode->x, currentSolutionNode->y);

                    pointList.push_back( point );

                    if ( currentSolutionNode->child )
                    {
                        currentSolutionNode = currentSolutionNode->child;
                    }
                    else
                    {
                        break;
                    }
                }

                // delete nodes that are needed for the solution.
                freeSolutionNodes( );

                return;
            }
            else // Not Goal
            {
                successors.clear( );

                bool successorsAdded;

                if ( node->parent )
                {
                    successorsAdded = node->getSuccessors( successors, map, node->parent );
                }
                else
                {
                    successorsAdded = node->getSuccessors( successors, map, nullptr );
                }

                if ( !successorsAdded )
                {
                    for ( Node *succesor: successors )
                    {
                        delete succesor;
                    }

                    successors.clear( );

                    delete node;

                    freeAllNodes( );

                    state = SearchState::OUT_OF_MEMORY;
                    return;
                }

                for ( Node *nodeSuccessor: successors )
                {
                    float valueGSuccessor = node->g + node->getCost( nodeSuccessor, map );

                    typename std::vector <Node *>::iterator openListResult;

                    for ( openListResult = openList.begin( ); openListResult != openList.end( ); openListResult += 1 )
                    {
                        if (( *openListResult )->isSameState( nodeSuccessor ))
                        {
                            break;
                        }
                    }

                    if ( openListResult != openList.end( ))
                    {
                        if (( *openListResult )->g <= valueGSuccessor )
                        {
                            delete nodeSuccessor;

                            continue;
                        }
                    }

                    typename std::vector <Node *>::iterator closedListResult;

                    for ( closedListResult = closedList.begin( );
                          closedListResult != closedList.end( ); closedListResult += 1 )
                    {
                        if (( *closedListResult )->isSameState( nodeSuccessor ))
                        {
                            break;
                        }
                    }

                    if ( closedListResult != closedList.end( ))
                    {
                        if (( *closedListResult )->g <= valueGSuccessor )
                        {
                            delete nodeSuccessor;

                            continue;
                        }
                    }

                    nodeSuccessor->parent = node;
                    nodeSuccessor->g = valueGSuccessor;
                    nodeSuccessor->h = nodeSuccessor->goalDistanceEstimate( goal );
                    nodeSuccessor->f = nodeSuccessor->g + nodeSuccessor->h;

                    if ( closedListResult != closedList.end( ))
                    {
                        ( *closedListResult )->parent = nodeSuccessor->parent;
                        ( *closedListResult )->g = nodeSuccessor->g;
                        ( *closedListResult )->h = nodeSuccessor->h;
                        ( *closedListResult )->f = nodeSuccessor->f;

                        delete nodeSuccessor;

                        openList.push_back(( *closedListResult ));

                        closedList.erase( closedListResult );

                        std::push_heap( openList.begin( ), openList.end( ), HeapCompare( ));
                    }
                    else if ( openListResult != openList.end( ))
                    {
                        ( *openListResult )->parent = nodeSuccessor->parent;
                        ( *openListResult )->g = nodeSuccessor->g;
                        ( *openListResult )->h = nodeSuccessor->h;
                        ( *openListResult )->f = nodeSuccessor->f;

                        delete nodeSuccessor;

                        std::make_heap( openList.begin( ), openList.end( ), HeapCompare( ));
                    }
                    else
                    {
                        openList.push_back( nodeSuccessor );

                        std::push_heap( openList.begin( ), openList.end( ), HeapCompare( ));
                    }
                }

                closedList.push_back( node );
            }
        }

        return;
    }
}

Doryen::Geometry::Point2D<> Doryen::Algorithms::Pathfinding::AStar::walk()
{
	if (state != SearchState::SUCCEEDED)
	{
		throw Doryen::Exceptions::IllegalMethodCall(
				"Illegal Method Call in Walk");
	}
	else
	{
		// Recordemos, que nuestro vector de puntos {pointList}
		// almacena los puntos de acuerdo a la siguiente estructura
        // Punto inicial -> Punto final
        // Por ende, es necesario obtener el primer elemento he ir
        // eliminandolo, con el fin de recorrer de forma lógica la
        // ruta.

        // Obtenemos el primer elemento
		Doryen::Geometry::Point2D point = pointList.front();
        // Eliminamos el primer elemento
        pointList.pop_front( );

        return point;
    }
}

bool Doryen::Algorithms::Pathfinding::AStar::isEmpty( ) const
{
    return pointList.empty( );
}

void Doryen::Algorithms::Pathfinding::AStar::reverse( )
{

}

int Doryen::Algorithms::Pathfinding::AStar::size( ) const
{
    return pointList.size( );
}

Doryen::Geometry::Point2D<> Doryen::Algorithms::Pathfinding::AStar::getPoint2DAt(const int index)
{
	if (state != SearchState::SUCCEEDED)
	{
		throw Doryen::Exceptions::IllegalMethodCall(
				"Illegal Method Call in getPoint2D");
	}
	else
	{
		return pointList.at(index);
	}
}

Doryen::Geometry::Point2D<> Doryen::Algorithms::Pathfinding::AStar::getOriginPoint2D() const
{
	return pointList.front();
}

Doryen::Geometry::Point2D<> Doryen::Algorithms::Pathfinding::AStar::getDestinationPoint2D() const
{
	return pointList.back();
}

void Doryen::Algorithms::Pathfinding::AStar::freeAllNodes( )
{
    for ( Doryen::Algorithms::Node *node: openList )
    {
        delete node;
    }

    openList.clear( );

    for ( Doryen::Algorithms::Node *node: closedList )
    {
        delete node;
    }

    closedList.clear( );

    delete goal;
}

void Doryen::Algorithms::Pathfinding::AStar::freeUnusedNodes( )
{
    for ( Doryen::Algorithms::Node *node: openList )
    {
        if ( !node->child )
        {
            delete node;
        }
    }

    openList.clear( );

    for ( Doryen::Algorithms::Node *node: closedList )
    {
        if ( !node->child )
        {
            delete node;
        }
    }

    closedList.clear( );
}

void Doryen::Algorithms::Pathfinding::AStar::freeSolutionNodes( )
{
    Doryen::Algorithms::Node *node = start;

    if ( start->child )
    {
        do
        {
            Doryen::Algorithms::Node *del = node;

            node = node->child;

            delete del;
        }
        while ( node != goal );

        delete goal;
    }
    else
    {
        delete start;
        delete goal;
    }
}

bool Doryen::Algorithms::Pathfinding::AStar::findPath( )
{
    if ( state == SearchState::SUCCEEDED )
    {
        return true;
    }
    else
    {
        return false;
    }
}
