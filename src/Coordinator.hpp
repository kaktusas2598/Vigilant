#ifndef __Coordinator__
#define __Coordinator__

#include "ECS.hpp"

namespace Vigilant {
    
    /*
     * Ties up entity, component and system managers and acts an orchestrator
     * for the entity component system(ECS)  
     */
    class Coordinator {
        public:
            static Coordinator* Instance() {
                if (sInstance == nullptr) {
                    sInstance =  new Coordinator();
                    return sInstance;
                }
                return sInstance;
            }

            void init() {
                entityManager = std::make_unique<EntityManager>();
                componentManager = std::make_unique<ComponentManager>();
                systemManager = std::make_unique<SystemManager>();
            }

            Entity createEntity() {
                return entityManager->createEntity();
            }

            void destoyEntity(Entity entity) {
                entityManager->destroyEntity(entity);
                componentManager->entityDestroyed(entity);
                systemManager->entityDestroyed(entity);
            }

            template<typename T>
            void registerComponent() {
                componentManager->registerComponent<T>();
            }

            template<typename T>
            void addComponent(Entity entity, T component) {
                componentManager->addComponent(entity, component);
                
                // Update entities signature
                auto signature = entityManager->getSignature(entity);
                signature.set(componentManager->getComponentType<T>(), true);
                entityManager->setSignature(signature, entity);

                systemManager->entitySignatureChanged(entity, signature);
            }

            template<typename T>
            void removeComponent(Entity entity, T component) {
                componentManager->removeComponent(entity, component);
                
                // Update entities signature
                auto signature = entityManager->getSignature(entity);
                signature.set(componentManager->getComponentType<T>(), false);
                entityManager->setSignature(signature, entity);

                systemManager->entitySignatureChanged(entity, signature);
            }

            template<typename T>
            T& getComponent(Entity entity) {
                return componentManager->getComponent<T>(entity);
            }

            template<typename T>
            ComponentType getComponentType() {
                return componentManager->getComponentType<T>();
            }

            template<typename T>
            std::shared_ptr<T> registerSystem() {
                return systemManager->registerSystem<T>();
            }

            template<typename T>
            void setSystemSignature(Signature signature) {
                systemManager->setSignature<T>(signature);
            }
            
        private:
            Coordinator() {}

            static Coordinator* sInstance;
            std::unique_ptr<EntityManager> entityManager;
            std::unique_ptr<ComponentManager> componentManager;
            std::unique_ptr<SystemManager> systemManager;
    };

    typedef Coordinator TheCoordinator;
}

#endif // __Coordinator__