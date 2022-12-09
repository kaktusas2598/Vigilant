#ifndef __TileLayer__
#define __TileLayer__

#include <vector>
#include "Layer.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

    struct TileSet;

    class TileLayer : public Layer {
        public:
            TileLayer(int tilesize, double scale, const std::vector<TileSet> &tilesets);

            virtual void update();
            virtual void render();

            void setTileIDs(const std::vector<std::vector<int>> &data) { tileIDs = data; }
            void setTileSize(int tilesize) { tileSize = tilesize; }
            void setMapWidth(int w) { mapWidth = w; }
            void setMapHeight(int h) { mapHeight = h; }

            int getTileSize() const { return tileSize; }
            double getScale() const { return scale; }
            int getMapWidth() const { return mapWidth; }
            int getMapHeight() const { return mapHeight; }
            const std::vector<std::vector<int>>& getTileIDs() { return tileIDs; }
            Vector2D getPosition() const { return position; }

			void setVisible(bool visible) { isVisible = visible; }
			bool getVisible() { return isVisible; }

        private:
            int tileSize;
            double scale;

            const std::vector<TileSet> &tileSets;

            // For scrolling maps
            Vector2D position;
            Vector2D velocity;

            // Useful for collision layer
            bool isVisible;

            // Tilesheet properties
            int numColumns;
            int numRows;

            int mapHeight;
            int mapWidth;

            std::vector<std::vector<int>> tileIDs;

            TileSet getTilesetByID(int tileID);
    };
}

#endif // __TileLayer__
