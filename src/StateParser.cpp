#include "StateParser.hpp"
#include "ErrorHandler.hpp"
#include "TextureManager.hpp"
#include "EntityFactory.hpp"

namespace Vigilant {
    bool StateParser::parseState(const char* stateFile, std::string stateID, std::vector<IEntity*> *pEntities,  std::vector<std::string> *pTextureIDs) {
        TiXmlDocument xmlDoc;

        if (!xmlDoc.LoadFile(stateFile)) {
            exitWithError("Could not load XML state file");
        }

        TiXmlElement *root = xmlDoc.RootElement();

        // Find state
        TiXmlElement *stateRoot = 0;
        for (TiXmlElement *e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == stateID) {
                stateRoot = e;
            }
        }

        TiXmlElement *textureRoot = 0;
        for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("TEXTURES")) {
                textureRoot = e;
            }
        }
        parseTextures(textureRoot, pTextureIDs);

        TiXmlElement *objectRoot = 0;
        for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("OBJECTS")) {
                objectRoot = e;
            }
        }
        parseObjects(objectRoot, pEntities);

        return true;
    }

    void StateParser::parseTextures(TiXmlElement* pStateRoot, std::vector<std::string> *pTextureIDs) {
        for (TiXmlElement *e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            std::string filename = e->Attribute("filename");
            std::string id = e->Attribute("ID");
            pTextureIDs->push_back(id);
            TheTextureManager::Instance()->load(filename, id);
        }
    }

    void StateParser::parseObjects(TiXmlElement* pStateRoot, std::vector<IEntity*> *entities) {
        for (TiXmlElement *e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            int x,y, width, height, numFrames, callbackID, animSpeed;
            std::string textureID;

            e->Attribute("x", &x);
            e->Attribute("y", &y);
            e->Attribute("width", &width);
            e->Attribute("height", &height);
            e->Attribute("numFrames", &numFrames);
            e->Attribute("callbackID", &callbackID);
            e->Attribute("animSpeed", &animSpeed);
            textureID = e->Attribute("textureID");

            IEntity* entity = TheEntityFactory::Instance()->create(e->Attribute("type"));
            entity->load(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed));
            entities->push_back(entity);
        }
    }
}