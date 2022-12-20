#ifndef __Node__
#define __Node__

#include <vector>

namespace Vigilant {

    // Used to represent tiled map as graph for pathfinding algorithm
    struct Node {
        bool solid = false; // obstacle or not
        bool visited = false; // was this path searched before
        float globalGoal; // Distance to goal so far
        float localGoal;
        int x; // position on map
        int y;
        std::vector<Node*> neighbours; // connecting tiles
        Node* parent;
    };

    // TODO: hook up with TileLayer and Entity somehow
    // Load map into graph in behaviour component?
    //
    //Below is example code for later
    // Example:
    Node* nodes = nullptr;
    // This will be numColumns and num rows
    int mapWidth = 32;
    int mapHeight = 32;

    // On the state which has level set on create
    notes = new Node[mapWidth * mapHeight];
    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++) {
            nodes[y * mapWidth + x].x = x;
            nodes[y * mapWidth + x].y = y;
            nodes[y * mapWidth + x].obstacle = false;// TODO: probably best to load this after level is created and set these
            nodes[y * mapWidth + x].visited = false;
            nodes[y * mapWidth + x].parent = nullptr;
        }

    // Create Connections
    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++) {
            // Add north neighnour if there is one
            if (y > 0)
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[(y - 1) * mapWidth + x]);
            // Add south neighbour
            if (y < mapHeight + 1)
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[(y + 1) * mapWidth + x]);
            // Add west neighnour
            if (x > 0)
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[y * mapWidth + x - 1]);
            // Add east neighbour
            if (x < mapWidth + 1)
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[y * mapWidth + x + 1]);
        }

    // TODO: set parents
    // start and end locations?

    // Dont forget to deallocate this on state exit!!!
    delete[] nodes;
}

#endif // __Node__
