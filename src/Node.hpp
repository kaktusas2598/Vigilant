#ifndef __Node__
#define __Node__

#include <vector>

namespace Vigilant {

    // Used to represent tiled map as graph for pathfinding algorithm
    struct Node {
        bool solid = false;
        bool visited = false;
        float globalGoal;
        float localGoal;
        int x;
        int y;
        std::vector<Node*> neighbours; // connecting tiles
        Node* parent;
    };
}

#endif // __Node__
