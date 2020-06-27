#include "Doryen/Algorithms/Util/Node.hpp"


Doryen::Algorithms::Node::Node( )
{
    x = 0;
    y = 0;

    g = 0;
    h = 0;
    f = 0;
}

Doryen::Algorithms::Node::Node( int X, int Y )
{
    x = X;
    y = Y;

    g = 0;
    h = 0;
    f = 0;
}

float Doryen::Algorithms::Node::goalDistanceEstimate(
        Doryen::Algorithms::Node *nodeGoal )
{
    return std::abs(x - nodeGoal->x) + std::abs(y - nodeGoal->y);
}

float Doryen::Algorithms::Node::getCost( Doryen::Algorithms::Node *successor,
                                         Doryen::Map &map )
{
    if (map.isWalkable(successor->x, successor->y))
    {
        // Low cost.
        return 1;
    }
    else
    {
        // High cost.
        return 9;
    }
}

bool Doryen::Algorithms::Node::getSuccessors( std::vector <Node *> &successors,
                                              Doryen::Map &map, Doryen::Algorithms::Node *nParent )
{
    int parentX = -1;
    int parentY = -1;

    if (nParent)
    {
        parentX = nParent->x;
        parentY = nParent->y;
    }

    if (map.isWalkable(x + 1, y) && !(parentX == x + 1 && parentY == y))
    {
        successors.push_back(new Node(x + 1, y));
    }

    if (map.isWalkable(x - 1, y) && !(parentX == x - 1 && parentY == y))
    {
        successors.push_back(new Node(x - 1, y));
    }

    if (map.isWalkable(x, y + 1) && !(parentX == x && parentY == y + 1))
    {
        successors.push_back(new Node(x, y + 1));
    }

    if (map.isWalkable(x, y - 1) && !(parentX == x && parentY == y - 1))
    {
        successors.push_back(new Node(x, y - 1));
    }

    return true;
}

bool Doryen::Algorithms::Node::isGoal( Doryen::Algorithms::Node *nodeGoal )
{
    return x == nodeGoal->x && y == nodeGoal->y;
}

bool Doryen::Algorithms::Node::isSameState( Doryen::Algorithms::Node *node )
{
    return (x == node->x) && (y == node->y);
}
