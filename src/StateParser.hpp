#include <iostream>
#include <vector>
// TODO: Replace with TinyXML2!
#include "tinyxml.h" 

namespace Vigilant {
   
    class IEntity;

	/*! \brief IEngine
	 *         Main Engine Class.
	 *
	 * Parses XML state file using tinyxml, for example of required structure look at state.xml file
	 */
    class StateParser {
        public:
            bool parseState(const char* stateFile, std::string stateID,
            std::vector<IEntity*> *pEntities,  std::vector<std::string> *pTextureIDs);
        private:
            void parseObjects(TiXmlElement* pStateRoot,
            std::vector<IEntity*> *pEntities);
            void parseTextures(TiXmlElement* pStateRoot,
            std::vector<std::string> *pTextureIDs);
    };
}