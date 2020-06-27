#ifndef LIBTCOD_NODE_HPP
#define LIBTCOD_NODE_HPP

#include <vector>
#include <cmath>

#include "Doryen/FOV/Fov.hpp"

namespace Doryen
{
    namespace Algorithms
    {
        class Node
        {

        public:

            int x;
            int y;

            float g;
            float h;
            float f;

            Node *parent = nullptr;
            Node *child = nullptr;

            Node();

            Node(int X, int Y);

            float goalDistanceEstimate(Node *nodeGoal);

            float getCost( Doryen::Algorithms::Node *successor, Doryen::Map &map );

            bool getSuccessors( std::vector <Node *> &successors,
                                Doryen::Map &map, Node *nParent);

            bool isGoal(Node *nodeGoal);

            bool isSameState(Node *node);

        };
    }
}

#endif //LIBTCOD_NODE_HPP
