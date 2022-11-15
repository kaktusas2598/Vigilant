#ifndef __LEVEL__
#define __LEVEL__

#include <string>
#include <vector>
#include "Layer.hpp"

namespace Vigilant {

    struct TileSet {
        int firstGridID;
        int tileWidth;
        int tileHeight;
        int spacing;
        int margin;
        int width;
        int height;
        int numColumns;
        std::string name;
    };

    class Level {
        public:
            ~Level();

            void update();
            void render();

            std::vector<TileSet>* getTilesets() {return &tileSets; }
            std::vector<Layer*>* getLayers() { return &layers; }
        private:
            friend class LevelParser;
            Level() {} // Constructor is private to ensure Level is only created by LevelParser

            std::vector<TileSet> tileSets;
            std::vector<Layer*> layers;
    };
}

#endif // __LEVEL__