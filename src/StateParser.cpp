#include "StateParser.hpp"

#include "ErrorHandler.hpp"
#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "EntityManager.hpp"
#include "ScriptEngine.hpp"
#include "GameState.hpp"

#include "Logger.hpp"

namespace Vigilant {
    bool StateParser::loadStates(const char* stateFile, std::vector<GameState*> *gameStates) {
        TiXmlDocument xmlDoc;

        if (!xmlDoc.LoadFile(stateFile)) {
            exitWithError("Could not load XML state file");
        }

        TiXmlElement *root = xmlDoc.RootElement();

        for (TiXmlElement *e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            GameState* state = new GameState(e->Value());
            std::string nextId = e->Attribute("next");
            state->setNextID(nextId);
            gameStates->push_back(state);
        }


        return true;
    }

    bool StateParser::parseState(const char* stateFile, std::string stateID, std::vector<std::string> *pTextureIDs, std::vector<std::string> *pSoundsIDs) {
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

        TiXmlElement *fontRoot = 0;
        for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("FONTS")) {
                fontRoot = e;
            }
        }
        if (fontRoot != 0)
            parseFonts(fontRoot);

        TiXmlElement *soundRoot = 0;
        for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("SOUNDS")) {
                soundRoot = e;
            }
        }

        if (soundRoot != 0)
            parseSounds(soundRoot, pSoundsIDs);

        TiXmlElement *scriptRoot = 0;
        for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            if (e->Value() == std::string("SCRIPTS")) {
                scriptRoot = e;
            }
        }

        if (scriptRoot != 0)
            parseScripts(scriptRoot);

        // TiXmlElement *mapRoot = 0;
        // for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        //     if (e->Value() == std::string("MAPS")) {
        //         mapRoot = e;
        //     }
        // }

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

    // TODO: for not state will only parse fonts and won't clean them up, I know this is bad, don't judge me
    void StateParser::parseFonts(TiXmlElement* pStateRoot) {
        Logger::Instance()->info("Loading fonts.");
        for (TiXmlElement *e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            std::string filename = e->Attribute("filename");
            std::string id = e->Attribute("ID");
            TheTextureManager::Instance()->loadFont(filename, id);
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

    void StateParser::parseScripts(TiXmlElement* pScriptRoot) {
        Logger::Instance()->info("Initialising scripts.");
        for (TiXmlElement *e = pScriptRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
            std::string name, fileName, type;

            name = e->Attribute("name");
            fileName = e->Attribute("filename");
            type = e->Attribute("type");

            ScriptEngine::Instance()->init(fileName);
            if (type == "entity") {
            }
        }
    }

    // This is the most basic approach so far, only loads first found map
    // But maybe 1 map/level is all we want?
    std::string StateParser::getMapName(const char* stateFile, std::string stateID) {
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

        if (stateRoot != 0) {
            TiXmlElement *mapRoot = 0;
            for (TiXmlElement *e = stateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
                if (e->Value() == std::string("MAPS")) {
                    mapRoot = e;
                }
            }

             if (mapRoot != 0) {
                for (TiXmlElement *e = mapRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
                    std::string fileName;

                    fileName = e->Attribute("filename");
                    return fileName;
                }
             }
        }

        return std::string();
     }
}
