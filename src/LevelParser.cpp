#include "LevelParser.hpp"
#include "Level.hpp"
#include "TileLayer.hpp"
#include "ObjectLayer.hpp"
#include "TextureManager.hpp"
#include "EntityFactory.hpp"
#include "IEntity.hpp"
#include "LoaderParams.hpp"
#include <string>

namespace Vigilant {

    Level* LevelParser::parseLevel(const char* levelFile) {
        TiXmlDocument levelDoc;
        levelDoc.LoadFile(levelFile);

        Level* level = new Level(); 

        TiXmlElement* root = levelDoc.RootElement();
        root->Attribute("tilewidth", &tileSize);
        root->Attribute("width", &width);
        root->Attribute("height", &height);

        // Parse properties for entity texture sources
        for (TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("properties")) {
                parseTextures(e);
            }
        }

        for (TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("tileset")) {
                parseTileSets(e, level->getTilesets());
            }
        }

        // Parse object and tile layers
        for (TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("objectgroup") || e->Value() == std::string("layer")) {
                if (e->FirstChildElement()->Value() == std::string("object")) {
                    parseObjectLayer(e, level->getLayers());
                } else if (e->FirstChildElement()->Value() == std::string("data")) {
                    parseTileLayer(e, level->getLayers(), level->getTilesets());
                }
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

            // Parsing CSV map data, probably not the best solution, need to investigate how much memory this uses
            std::vector<int> layerRow;
            std::string row;
            std::istringstream tokenStream(t);
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

    void LevelParser::parseTextures(TiXmlElement *textureRoot) {
        for (TiXmlElement* e = textureRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            TheTextureManager::Instance()->load(e->Attribute("value"), e->Attribute("name"));
        }
    }

    void LevelParser::parseObjectLayer(TiXmlElement *objectRoot, std::vector<Layer*>* layers) {
        ObjectLayer* objLayer = new ObjectLayer();

        for (TiXmlElement* e = objectRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("object")) {
                int x, y, width, height, numFrames, callbackId = 0;
                std::string textureId;

                e->Attribute("x", &x);
                e->Attribute("y", &y);
                e->Attribute("width", &width);
                e->Attribute("height", &height);
                IEntity* entity = TheEntityFactory::Instance()->create(e->Attribute("class"));

                for (TiXmlElement* properties = e->FirstChildElement(); properties != NULL; properties = properties->NextSiblingElement()) {
                    if (properties->Value() == std::string("properties")) {
                        for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement()) {
                            if (property->Attribute("name") == std::string("numFrames")) {
                                property->Attribute("value", &numFrames);
                            } else if (property->Attribute("name") == std::string("textureID")) {
                                textureId = property->Attribute("value");
                            } else if (property->Attribute("name") == std::string("callbackID")) {
                                property->Attribute("value", &callbackId);
                            }
                            // TODO: Parse more properties like animation speed and so on
                        }
                    }
                }
                entity->load(new LoaderParams(x, y, width, height, textureId, numFrames, callbackId));
                objLayer->getEntities()->push_back(entity);
            }
        }
        layers->push_back(objLayer);
    }
}