#ifndef __ECS__
#define __ECS__

#include <stdint.h>
#include <bitset>
#include <array>
#include <queue>
#include <unordered_map>
#include <assert.h>
#include <memory>
#include <set>

namespace Vigilant {

    /*
     * Entity Component System (ECS) 
     */

    using Entity = std::uint32_t;

    const Entity MAX_ENTITIES = 5000;

    using ComponentType = std::uint8_t;
    const ComponentType MAX_COMPONENTS = 32;

    // Lets track which components Entity has
    using Signature = std::bitset<MAX_COMPONENTS>;

    class EntityManager {
        public:
            EntityManager() {
                // initialize entity queue
                for (Entity e = 0; e < MAX_ENTITIES; ++e) {
                    availableEntities.push(e);
                }
            }

            Entity createEntity() {
                assert(livingEntityCount < MAX_ENTITIES && "Too many entities!");

                Entity id = availableEntities.front();
                availableEntities.pop();
                ++livingEntityCount;

                return id;
            }

            void destroyEntity(Entity entity) {
                assert(entity < MAX_ENTITIES && "Entity id out of range");

                // invalidate entity's signature
                signatures[entity].reset();

                // put destroyed id at the back of the queue
                availableEntities.push(entity);
                --livingEntityCount;
            }

            void setSignature(Signature signature, Entity entity) {
                assert(entity < MAX_ENTITIES && "Entity id out of range");

                signatures[entity] = signature;
            }

            Signature getSignature(Entity entity) const {
                assert(entity < MAX_ENTITIES && "Entity id out of range");

                return signatures[entity];
            }
        private:
            std::queue<Entity> availableEntities{};
            std::array<Signature, MAX_ENTITIES> signatures{};
            uint32_t livingEntityCount{};
    };

    class IComponentArray {
        public:
            virtual ~IComponentArray() = default;
            virtual void entityDestroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
        public:
            void insertData(Entity entity, T component) {

                assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once.");

                // put new entry at the end amd update the maps
                size_t newIndex = size;
                entityToIndexMap[entity] = newIndex;
                indexToEntityMap[newIndex] = entity;
                componentArray[newIndex] = component;
                ++size;
            }

            // Remove entity's component if it exists
            void removeData(Entity entity) {
                assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Cannot remove non-existent component");

                // to maintain packed array, copy element at the end into deleted's place
                size_t removedEntityIndex = entityToIndexMap[entity];
                size_t lastElementIndex = size - 1;
                componentArray[removedEntityIndex] = componentArray[lastElementIndex];

                // update map to point to new spot
                Entity lastElementEntity = indexToEntityMap[lastElementIndex];
                entityToIndexMap[lastElementEntity] = removedEntityIndex;
                indexToEntityMap[removedEntityIndex] = lastElementEntity;

                entityToIndexMap.erase(entity);
                indexToEntityMap.erase(lastElementIndex);
            }

            // Return reference of entity's component
            T& getData(Entity entity) {
                assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Component does not exist");

                return componentArray[entityToIndexMap[entity]];
            }

            void entityDestroyed(Entity entity) override {
                if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
                    removeData(entity);
                }
            }

        private:
            // Packed array of components, set to maximum amount so that each entity has unique place
            std::array<T, MAX_ENTITIES> componentArray{};

            std::unordered_map<Entity, size_t> entityToIndexMap{};
            std::unordered_map<size_t, Entity> indexToEntityMap{};
            // total valid entries in array
            size_t size;
    };

    class ComponentManager {
        public:
            template<typename T>
            void registerComponent() {
                const char* typeName = typeid(T).name();
                assert(componentTypes.find(typeName) == componentTypes.end() && "Component type already registered");
                
                componentTypes.insert({typeName, nexComponentType});
                componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
                //increase so that he next registered component have a different id!
                ++nexComponentType;
            }

            template<typename T>
            ComponentType getComponentType() {
                const char* typeName = typeid(T).name();
                assert(componentTypes.find(typeName) != componentTypes.end() && "Component needs to be registered first");
                return componentTypes[typeName];
            }

            template<typename T>
            void addComponent(Entity entity, T component) {
                getComponentArray<T>()->insertData(entity, component);
            }

            template<typename T>
            void removeComponent(Entity entity) {
                getComponentArray<T>()->removeData(entity);
            }

            template<typename T>
            T& getComponent(Entity entity) {
                return getComponentArray<T>()->getData(entity);
            }

            void entityDestroyed(Entity entity) {
                // Notify each component array that an entity has been destroyed and each reference to it will be removed
                for (auto const& pair: componentArrays) {
                    auto const& component = pair.second;
                    component->entityDestroyed(entity);
                }
            }
        private:
            std::unordered_map<const char*, ComponentType> componentTypes{};
            std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};
            ComponentType nexComponentType{};

            template<typename T>
            std::shared_ptr<ComponentArray<T>> getComponentArray() {
                const char* typeName = typeid(T).name();
                assert(componentTypes.find(typeName) != componentTypes.end() && "Component needs to be registered first");

                return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
            }
    };


    class System {
        public:
            std::set<Entity> entities;
    };

    class SystemManager {
        public:
            template<typename T>
            std::shared_ptr<T> registerSystem() {
                const char* typeName = typeid(T).name();
                assert(systems.find(typeName) == systems.end() && "System already registered");
                
                auto system = std::make_shared<T>();
                systems.insert({typeName, system});
                return system;
            }

            template<typename T>
            void setSignature(Signature signature) {
                const char* typeName = typeid(T).name();
                assert(systems.find(typeName) != systems.end() && "System needs to be registered first");
                signatures.insert({typeName, signature});
            }

            void entityDestroyed(Entity entity) {
                // Erase entity from system lists
                for (auto const& pair: systems) {
                    auto const& system = pair.second;
                    system->entities.erase(entity);
                }
            }

            void entitySignatureChanged(Entity entity, Signature entitySignature) {
               for (auto const& pair: systems) {
                    auto const& type = pair.first;
                    auto const& system = pair.second;
                    auto const& systemSignature = signatures[type];

                    // if entity signature matches system's insert it, else delete it from system
                    if((entitySignature & systemSignature) == systemSignature) {
                        system->entities.insert(entity);
                    } else {
                        system->entities.erase(entity);
                    }
                    
                } 
            }
        private:
            std::unordered_map<const char*, Signature> signatures{};
            std::unordered_map<const char*, std::shared_ptr<System>> systems{};
    };
}

#endif // __ECS__