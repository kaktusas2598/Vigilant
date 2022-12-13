#include <iostream>
#include <vector>
// TODO: Replace with TinyXML2!
// Also move this to includes to reduce clutter and make sure to include TinyXML to source list because it needs to be compiled
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
            void parseObjects(TiXmlElement* pStateRoot, std::vector<Entity*> *entities);
            void parseTextures(TiXmlElement* pStateRoot, std::vector<std::string> *pTextureIDs);
            void parseFonts(TiXmlElement* pStateRoot);
            void parseSounds(TiXmlElement* pStateRoot, std::vector<std::string> *pSoundIDs);
            void parseScripts(TiXmlElement* pScriptRoot, std::vector<Entity*> *entities);
    };
}
