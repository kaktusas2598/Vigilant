#ifndef __TileLayer__
#define __TileLayer__

#include <vector>
#include "Layer.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

    struct TileSet;

    class TileLayer : public Layer {
        public:
            TileLayer(int tilesize, const std::vector<TileSet> &tilesets);

            virtual void update();
            virtual void render();

            void setTileIDs(const std::vector<std::vector<int>> &data) { tileIDs = data; }
            void setTileSize(int tilesize) { tileSize = tilesize; }
            void setMapWidth(int w) { mapWidth = w; }
            void setMapHeight(int h) { mapHeight = h; }

            int getTileSize() { return tileSize; }
            const std::vector<std::vector<int>>& getTileIDs() { return tileIDs; }
            Vector2D getPosition() const { return position; }

        private:
            // Tilesheet properties
            int numColumns;
            int numRows;
            int tileSize;

            int mapHeight;
            int mapWidth;            

            // For scrolling maps
            Vector2D position;
            Vector2D velocity;

            const std::vector<TileSet> &tileSets;
            std::vector<std::vector<int>> tileIDs;

            TileSet getTilesetByID(int tileID);
    };
}

#endif // __TileLayer__