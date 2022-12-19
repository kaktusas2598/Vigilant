#include <iostream>
#include <vector>
// TODO: Replace with TinyXML2!
#include "tinyxml.h"

namespace Vigilant {

    class Entity;
    class GameState;

    /***
     * Parses XML state file using tinyxml, for example of required structure look at state.xml file
     */
    class StateParser {
        public:
            // Parse single state, load assets and script
            bool parseState(const char* stateFile, std::string stateID, std::vector<std::string> *pTextureIDs, std::vector<std::string> *pSoundsIDs);
            bool loadStates(const char* stateFile, std::vector<GameState*> *gameStates);
            std::string getMapName(const char* stateFile, std::string stateID);
        private:
            void parseObjects(TiXmlElement* pStateRoot, std::vector<Entity*> *entities);
            void parseTextures(TiXmlElement* pStateRoot, std::vector<std::string> *pTextureIDs);
            void parseFonts(TiXmlElement* pStateRoot);
            void parseSounds(TiXmlElement* pStateRoot, std::vector<std::string> *pSoundIDs);
            void parseScripts(TiXmlElement* pScriptRoot);
    };
}
