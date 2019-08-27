#ifndef LIBTCOD_NODE_HPP
#define LIBTCOD_NODE_HPP

#include <vector>

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

            float getCost(Node *successor);

            bool getSuccessors(std::vector<Node *> successors, Node *parent);

            bool getSuccessors(std::vector<Node *> successors);

            bool isGoal(Node *node);

            bool isSameState(Node *node);

        };
    }
}

#endif //LIBTCOD_NODE_HPP
