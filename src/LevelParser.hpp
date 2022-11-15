#ifndef __LevelParser__
#define __LevelParser__

#include "LevelParser.hpp"
#include "tinyxml.h" 
#include <vector>

namespace Vigilant {

    class Level;
    class Layer;
    struct TileSet;
    
    class LevelParser {
        public:
            Level* parseLevel(const char* levelFile);
        private:
            void parseTileSets(TiXmlElement *tilesetRoot, std::vector<TileSet>* tilesets);
            void parseTileLayer(TiXmlElement *tileElement, std::vector<Layer*>* layers, const std::vector<TileSet>* tilesets);

            int tileSize;
            int width;
            int height;
    };
        
}

#endif // __LevelParser__