#include "LevelParser.hpp"
#include "Level.hpp"
#include "TileLayer.hpp"
#include "TextureManager.hpp"
#include <string>

#include "rapidcsv.h"

namespace Vigilant {

    Level* LevelParser::parseLevel(const char* levelFile) {
        TiXmlDocument levelDoc;
        levelDoc.LoadFile(levelFile);

        Level* level = new Level(); 

        TiXmlElement* root = levelDoc.RootElement();
        root->Attribute("tilewidth", &tileSize);
        root->Attribute("width", &width);
        root->Attribute("height", &height);

        for (TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("tileset")) {
                parseTileSets(e, level->getTilesets());
            }
        }

        for (TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("layer")) {
                parseTileLayer(e, level->getLayers(), level->getTilesets());
            }
        }
        
        return level;
    }

    void LevelParser::parseTileSets(TiXmlElement *tilesetRoot, std::vector<TileSet>* tilesets) {
        // Load tileset textures
        TheTextureManager::Instance()->load(
            tilesetRoot->FirstChildElement()->Attribute("source"),
            tilesetRoot->Attribute("name")
        );

        TileSet tileset;
        tilesetRoot->FirstChildElement()->Attribute("width", &tileset.width);
        tilesetRoot->FirstChildElement()->Attribute("height", &tileset.height);
        tilesetRoot->Attribute("firstgid", &tileset.firstGridID);
        tilesetRoot->Attribute("tilewidth", &tileset.tileWidth);
        tilesetRoot->Attribute("tileheight", &tileset.tileHeight);
        tilesetRoot->Attribute("spacing", &tileset.spacing);
        tilesetRoot->Attribute("margin", &tileset.margin);
        tileset.name = tilesetRoot->Attribute("name");

        tileset.numColumns = tileset.width / (tileset.tileWidth + tileset.spacing);

        tilesets->push_back(tileset);

    }

    void LevelParser::parseTileLayer(TiXmlElement *tileElement, std::vector<Layer*>* layers, const std::vector<TileSet>* tilesets) {
            TileLayer* layer = new TileLayer(tileSize, *tilesets);

            std::vector<std::vector<int>> data; // tile data
            // std::string decodedIDs;
            TiXmlElement *dataNode;
            for (TiXmlElement* e = tileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
                if (e->Value() == std::string("data")) {
                    dataNode = e;
            }

            std::string t; // mine
            for (TiXmlNode* e = dataNode->FirstChild(); e != NULL; e = e->NextSibling()) {
                TiXmlText* text = e->ToText();
                /*std::string*/ t = text->Value();
                // decodedIDs = base64_decode(t);
            }

            // Parsing CSV map data
            std::vector<int> layerRow;
            std::string row;
            std::istringstream tokenStream(t);
            // Parse each CSV line, probably not the best solution, need to investigate how much memory this uses
            while(std::getline(tokenStream, row, ' ')) {
                layerRow.clear();
                std::istringstream rowStream(row);
                std::string tileId;
                while(std::getline(rowStream, tileId, ',')) {
                    layerRow.push_back(stoi(tileId));
                }
                data.push_back(layerRow); 
            }
            

            // uncompress zlib compression
            // uLongf numGids = width * height * sizeof(int);
            // long numGids = width * height * sizeof(int);
            // std::vector<unsigned> gids(numGids);
            // uncompress((Bytef*)&gids[0], &numGids,(const
            // Bytef*)decodedIDs.c_str(), decodedIDs.size());

            // std::vector<int> layerRow(width);
            // for (int j = 0; j < height; j++) {
            //     data.push_back(layerRow);
            // }

            // for (int rows = 0; rows < height; rows++) {
            //     for (int cols = 0; cols < width; cols++) {
            //         // data[rows][cols] = gids[rows * width + cols];
            //     }
            // }

            layer->setTileIDs(data);
            layers->push_back(layer);
        }
    }
}