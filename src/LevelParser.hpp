#ifndef __LevelParser__
#define __LevelParser__

//#include "LevelParser.hpp"
#include "tinyxml.h"
#include <vector>
#include <string>

namespace Vigilant {

    class Level;
    class Layer;
    class CollisionLayer;
    struct TileSet;
    class TileLayer;

    class LevelParser {
        public:
            Level* parseLevel(const char* levelFile);
        private:
            void parseTileSets(TiXmlElement *tilesetRoot, Level* level);
            void parseTextures(TiXmlElement *textureRoot, Level* level);
            void parseTileLayer(
                TiXmlElement *tileElement,
                std::vector<Layer*>* layers,
                const std::vector<TileSet>* tilesets,
                std::vector<TileLayer*>* collisionLayers
            );
            void parseObjectLayer(TiXmlElement *objectRoot, std::vector<Layer*>* layers);
            CollisionLayer* parseCollisionLayer(TiXmlElement* collisionRoot, std::vector<Layer*>* layers);

            int tileSize;
            int width;
            int height;
	    double scale;
    };

}

#endif // __LevelParser__
