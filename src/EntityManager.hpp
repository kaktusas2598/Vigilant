#ifndef __EntityManager__
#define __EntityManager__

#include "Entity.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>

namespace Vigilant {

	// To ensure this works with lua with no segfaults
	// Should hold ID for each entity and if Lua calls __gc it makes sure this ID is not active
	// So I am thinking instead of std::vector<Entity*>
	// I will do std::unordered_map<int, Entity*> ??
	//
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
                for (auto &e: entityMap) {
					e.second->update(deltaTime);
				}
                for (auto& e: entities)
                    e->update(deltaTime);
            }

            void render(float deltaTime) {
				for (auto &e: entityMap) {
					e.second->update(deltaTime);
				}

                for (auto& e: entities)
                    e->draw(deltaTime);
            }

            Entity* get(int id) const {
				auto pos = entityMap.find(id);
				if (pos == entityMap.end()) {
					return nullptr;
				} else {
					return pos->second;
				}
			}

			bool exists(int id) const {
				 if (entityMap.count(id) > 0) {
				 	 return true;
				 }
				 return false;
			}

            void refresh() {
                for (auto& e: entities) {
                    if (!e->isAlive()) {
                        entities.erase(std::remove(entities.begin(), entities.end(), e), entities.end());
                        // delete e;

                    }
                }
                // entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                //     [](std::shared_ptr<Entity> &e) {
                //         return !e->isAlive();
                //     }),
                //     std::end(entities));
            }

            void remove(int id) {
                livingCount--;
				// entityMap.erase(id);
				auto it = entities.begin();
				while(it != entities.end()) {
					// If entities is even number then delete it
					if ((int)(*it)->id->get() == id) {
						entities.erase(it);
                        return;
					}
                    it++;
				}
			}

            // Add new entity
            Entity* addEntity() {
                livingCount++;
                Entity* entity = new Entity();
                entities.push_back(entity);
                return entity;
				// std::shared_ptr<Entity> e{ entity };
                // entities.emplace_back(std::move(e));
                // return e.get();
            }

            // Add existing entity: for lua
            void addEntity(Entity* entity) {
                livingCount++;
                // Works but hard to remove entities and gets segfault
                // std::vector
				entities.push_back(entity);

                // No segfault but doesnt work??
				// std::unordered_map, sorted by uid
				// entityMap[entity->id->get()] = entity;
				// entityMap.insert(std::pair<int, Entity*>(entity->id->get(), entity));
                //auto inserted = entityMap.emplace(entity->id.get(), entity);
				//auto it = inserted.first; // iterator to created id/Entity pair
				//auto& e = *it->second; // created entity
				// std::shared_ptr<Entity> e{ entity };
                // entities.emplace_back(std::move(e));
            }

			// void addEntity(const Entity&& e) {
                // entities.push_back(std::move(e));
            // }


            // void addEntity(const Entity& entity) {
                // entities.push_back(entity);
            // }

            const std::vector<Entity*>& getEntities() { return entities; }
			static int livingCount;

        private:
            EntityManager() {}

            static EntityManager* s_pInstance;

            // Probably should use std::shared_ptr<Entity> instead
            // I tried this on Ubuntu and it does seem to solve memory problem and entities get deleted
            // but also getting segfault on Linux and I'm not sure if it's because of this class or
            // my use of unique pointers
            std::vector<Entity*> entities;
            std::unordered_map<int, Entity*> entityMap;
    };
}

#endif // __EntityManager__
