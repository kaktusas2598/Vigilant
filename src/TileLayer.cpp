#include "TileLayer.hpp"

#include "Engine.hpp"
#include "TextureManager.hpp"
#include "Level.hpp"

#include "Logger.hpp"

namespace Vigilant {
    TileLayer::TileLayer (int tilesize, const std::vector<TileSet> &tilesets) : tileSize(tilesize), tileSets(tilesets), position(0,0), velocity(0,0), isVisible(true) {
        // Number of tile columns and rows needed to fill the screen
        numColumns = TheEngine::Instance()->getScreenWidth() / tileSize;
        numRows = TheEngine::Instance()->getScreenHeight() / tileSize;

    }

    void TileLayer::update() {
        // For scrolling maps
        // if(position.getX() < ((mapWidth * tileSize) - TheEngine::Instance()->getGameWidth()) - tileSize) {
        //     velocity.setX(TheEngine::Instance()->getScrollSpeed());
        //     position += velocity;
        // } else {
        //     velocity.setX(0);
        // }
    }

    void TileLayer::render() {
	if (!isVisible) {
	    return;
	}

        int x, y, x2, y2 = 0;


	    // Tile map position based on camera - causes weird rendering issues
        position.setX(TheEngine::Instance()->camera.x);
        position.setY(TheEngine::Instance()->camera.y);

        x = position.getX() / tileSize;
        y = position.getY() / tileSize;

        x2 = int(position.getX()) % tileSize;
        y2 = int(position.getY()) % tileSize;

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numColumns; j++) {

                 // segfault if screen column bigger than level column
                 // also seg if i + y and camera.y affects map
                int id = tileIDs[i + y][j + x]; //i + y
                if (id == 0) {
                    continue;
                }

                //if(((j * tileSize) - x2) - TheEngine::Instance()->camera.x < -tileSize ||
                    //((j * tileSize) - x2) - TheEngine::Instance()->camera.x > TheEngine::Instance()->getScreenWidth()) {
                    //continue;
                //}

                TileSet tileset = getTilesetByID(id);
                id--;

                // TheTextureManager::Instance()->drawTile(tileset.name, tileset.margin, tileset.spacing, (j * tileSize) - x2, (i * tileSize) - y2,
                // tileSize, tileSize, (id - (tileset.firstGridID - 1)) / tileset.numColumns, (id - (tileset.firstGridID - 1)) % tileset.numColumns);

                // draw the tile into position while offsetting its x position by subtracting the camera position
                TheTextureManager::Instance()->drawTile(tileset.name, tileset.margin, tileset.spacing,
                    ((j * tileSize) - x2),// - TheEngine::Instance()->camera.x, //x
                    ((i * tileSize) - y2), //y
                    tileSize, tileSize, // width, height
                    (id - (tileset.firstGridID - 1)) / tileset.numColumns, // row
                    (id - (tileset.firstGridID - 1)) % tileset.numColumns); // column
            }
        }
    }

    TileSet TileLayer::getTilesetByID(int tileID) {
        for (size_t i = 0; i < tileSets.size(); i++) {
           if( i + 1 <= tileSets.size() - 1) {
                if(tileID >= tileSets[i].firstGridID && tileID < tileSets[i +1].firstGridID) {
                    return tileSets[i];
                }
            }
            else {
                return tileSets[i];
            }
        }
        Logger::Instance()->error("Could not find tileset, returning empty one.");
        TileSet t;
        return t;
    }
}
