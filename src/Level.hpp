#ifndef __LEVEL__
#define __LEVEL__

#include <string>
#include <vector>
#include "CollisionLayer.hpp"
#include "TileLayer.hpp"

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

            std::vector<TileLayer*>* getCollisionLayers() { return &collisionLayers; }
            const std::vector<TileLayer*>& getCollidableLayers() { return collisionLayers; }

            void setWidth(int w) { width = w; }
            void setHeight(int h) { height = h; }

            int getWidth() const { return width; }
            int getHeight() const { return height; }

            CollisionLayer* getCollisionLayer() { return collisionLayer; }
            void setCollisionLayer(CollisionLayer* colLayer) { collisionLayer = colLayer; }
        private:
            friend class LevelParser;
            Level() {} // Constructor is private to ensure Level is only created by LevelParser

			// level width an dheight in px
			int width;
			int height;

            std::vector<TileSet> tileSets;
            std::vector<Layer*> layers;
            // Collision layers defined per tile
            std::vector<TileLayer*> collisionLayers;
            // Collision layer defined by individual bounding boxes
            CollisionLayer* collisionLayer;
    };
}

#endif // __LEVEL__
