#include <iostream>
#include <vector>
// TODO: Replace with TinyXML2!
#include "tinyxml.h" 

namespace Vigilant {
   
    class Entity;

	/*! \brief Engine
	 *         Main Engine Class.
	 *
	 * Parses XML state file using tinyxml, for example of required structure look at state.xml file
	 */
    class StateParser {
        public:
            bool parseState(const char* stateFile, std::string stateID, std::vector<Entity*> *pEntities,  std::vector<std::string> *pTextureIDs, std::vector<std::string> *pSoundsIDs);
        private:
            void parseObjects(TiXmlElement* pStateRoot, std::vector<Entity*> *pEntities);
            void parseTextures(TiXmlElement* pStateRoot, std::vector<std::string> *pTextureIDs);
            void parseSounds(TiXmlElement* pStateRoot, std::vector<std::string> *pSoundIDs);
            void parseScripts(TiXmlElement* pScriptRoot, std::vector<std::string> *pScriptIDs);
    };
}