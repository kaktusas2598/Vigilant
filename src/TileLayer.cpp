#include "TileLayer.hpp"

#include "Engine.hpp"
#include "TextureManager.hpp"
#include "Level.hpp"
#include <iostream>

namespace Vigilant {
    TileLayer::TileLayer (int tilesize, const std::vector<TileSet> &tilesets) : tileSize(tilesize), tileSets(tilesets), position(0,0), velocity(0,0) {
        numColumns = TheEngine::Instance()->getScreenWidth() / tileSize;
        numRows = TheEngine::Instance()->getScreenHeight() / tileSize;
    }

    void TileLayer::update() {
        // Will be used for scrolling maps later
        position += velocity;
        // TEMPORARY FOR SCROLLING TEST, will have to delete as this will cause segfault
        velocity.setX(1);
    }

    void TileLayer::render() {
        int x, y, x2, y2;

        x = position.getX() / tileSize;
        y = position.getY() / tileSize;

        x2 = int(position.getX()) % tileSize;
        y2 = int(position.getY()) % tileSize;

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numColumns; j++) {
                int id = tileIDs[i + y][j + x]; // seems like segfault if screen column bigger than level column 
                if (id == 0) {
                    continue;
                }

                TileSet tileset = getTilesetByID(id);
                id--;

                TheTextureManager::Instance()->drawTile(tileset.name, tileset.margin, tileset.spacing, (j * tileSize) - x2, (i * tileSize) - y2,
                tileSize, tileSize, (id - (tileset.firstGridID - 1)) / tileset.numColumns, (id - (tileset.firstGridID - 1)) % tileset.numColumns);
            }
        }
    }

    TileSet TileLayer::getTilesetByID(int tileID) {
        for (int i = 0; i < tileSets.size(); i++) {
           if( i + 1 <= tileSets.size() - 1) {
                if(tileID >= tileSets[i].firstGridID && tileID < tileSets[i +1].firstGridID) {
                    return tileSets[i];
                }
            }
            else {
                return tileSets[i];
            }
        }
        std::cout << "did not find tileset, returning empty tileset\n";
        TileSet t;
        return t;
    }
}
