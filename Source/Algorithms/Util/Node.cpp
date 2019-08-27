#include "Algorithms/Util/Node.hpp"


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
