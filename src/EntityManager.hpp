#ifndef __EntityManager__
#define __EntityManager__

#include "Entity.hpp"
#include <vector>
#include <algorithm>

namespace Vigilant {
    
    class EntityManager {
        public:
            static EntityManager* Instance() {
                if(s_pInstance == 0) {
                    s_pInstance = new EntityManager();
                    return s_pInstance;
                }
                return s_pInstance;
            }

            void update(float deltaTime) {
                for (auto& e: entities)
                    e->update(deltaTime);
            }

            void render(float deltaTime) {
                for (auto& e: entities)
                    e->draw(deltaTime);
            }

            void refresh() {
                entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                    [](Entity* e) {
                        return !e->isAlive();
                    }),
                    std::end(entities));
            }

            // Add new entity
            Entity* addEntity() {
                Entity* entity = new Entity();
                entities.emplace_back(entity);
                return entity;
            }

            // Add existing entity: for lua
            void addEntity(Entity* entity) {
                entities.emplace_back(entity);
            }

            std::vector<Entity*> getEntities() { return entities; }

        private:
            EntityManager() {}

            static EntityManager* s_pInstance;
            std::vector<Entity*> entities;
    };
}

#endif // __EntityManager__