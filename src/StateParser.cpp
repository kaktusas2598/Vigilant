#include "StateParser.hpp"

#include "ErrorHandler.hpp"
#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "ScriptingEngine.hpp"
#include "EntityFactory.hpp"

#include "Logger.hpp"

namespace Vigilant {
    bool StateParser::parseState(const char* stateFile, std::string stateID, std::vector<Entity*> *pEntities,  std::vector<std::string> *pTextureIDs, std::vector<std::string> *pSoundsIDs) {
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

        TiXmlElement *soundRoot = 0;
        for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("SOUNDS")) {
                soundRoot = e;
            }
        }

        if (soundRoot != 0)
            parseSounds(soundRoot, pSoundsIDs);

        TiXmlElement *objectRoot = 0;
        for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("OBJECTS")) {
                objectRoot = e;
            }
        }

        if (objectRoot != 0)
            parseObjects(objectRoot, pEntities);

        TiXmlElement *scriptRoot = 0;
        for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("SCRIPTS")) {
                scriptRoot = e;
            }
        }

        if (scriptRoot != 0)
            parseScripts(scriptRoot, pEntities);

        return true;
    }

    void StateParser::parseTextures(TiXmlElement* pStateRoot, std::vector<std::string> *pTextureIDs) {
        Logger::Instance()->info("Loading textures.");
        for (TiXmlElement *e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            std::string filename = e->Attribute("filename");
            std::string id = e->Attribute("ID");
            pTextureIDs->push_back(id);
            TheTextureManager::Instance()->load(filename, id);
        }
    }

    void StateParser::parseSounds(TiXmlElement* pStateRoot, std::vector<std::string> *pSoundIDs) {
        Logger::Instance()->info("Loading sounds.");
        for (TiXmlElement *e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            std::string filename = e->Attribute("filename");
            std::string id = e->Attribute("ID");
            int type;
            SoundType soundType;
            e->Attribute("type", &type);
            switch (type) {
            case 0:
                soundType = SOUND_MUSIC;
                break;
            default:
                soundType = SOUND_SFX;
                break;
            }
            pSoundIDs->push_back(id);
            TheSoundManager::Instance()->load(filename, id, soundType);
        }
    }


    // TODO: Deprecate once loading from sripts have been implemented
    void StateParser::parseObjects(TiXmlElement* pStateRoot, std::vector<Entity*> *entities) {
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

            Entity* entity = TheEntityFactory::Instance()->create(e->Attribute("type"));
            entity->load(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed));
            entities->push_back(entity);
        }
    }

    
    void StateParser::parseScripts(TiXmlElement* pScriptRoot, std::vector<Entity*> *entities) {
        Logger::Instance()->info("Initialising scripts.");
        for (TiXmlElement *e = pScriptRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            std::string name, fileName, type;

            name = e->Attribute("name");
            fileName = e->Attribute("filename");
            type = e->Attribute("type");

            if (type == "entity") {
                // TODO: this is temporary way to parse scripts for initialising entities
                Entity* entity = new Entity();
                entity->loadScript(name, fileName);

                // Entity* entity = TheEntityFactory::Instance()->create(e->Attribute("type"));
                // entity->load(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed));
                entities->push_back(entity);
            }
        }
    }
}