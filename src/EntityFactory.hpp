#ifndef __EntityFactory__
#define __EntityFactory__

#include <string>
#include <map>
#include <iostream>

#include "IEntity.hpp"

/**
 * Distibuted factory pattern to make game entity creation more streamlined
 */
namespace Vigilant {

    class BaseCreator {
        public:
            virtual IEntity *createEntity() const = 0;
            virtual ~BaseCreator() {}
    };

    class EntityFactory {
        public:
            static EntityFactory* Instance() {
                if(pInstance == 0)
                {
                    pInstance = new EntityFactory();
                }
                
                return pInstance;
            }

            bool registerType(std::string typeID, BaseCreator *creator) {
                std::map<std::string, BaseCreator*>::iterator it = creators.find(typeID);

                // If type is already registered
                if (it != creators.end()) {
                    delete creator;
                    return false;
                }

                creators[typeID] = creator;
                return true;
            }

            IEntity* create(std::string typeID) {
                std::map<std::string, BaseCreator*>::iterator it = creators.find(typeID);

                if (it == creators.end()) {
                    std::cout << "Could not find entity type: " << typeID << std::endl;
                    return NULL;
                }

                BaseCreator* creator = (*it).second;
                return creator->createEntity();
            }

        private:
            static EntityFactory* pInstance;
            std::map<std::string, BaseCreator*> creators;
    };

    typedef EntityFactory TheEntityFactory;
}

#endif // __EntityFactory__