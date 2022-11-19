#ifndef __InputSystem__
#define __InputSystem__

#include "ECS.hpp"
#include "Coordinator.hpp"
#include "ComponentTypes.hpp"
#include "InputManager.hpp"

namespace Vigilant {

    class InputSystem : public System {
        public:
            void update(float deltaTime) {
                for (auto const& entity : entities) {
                    auto& transform = TheCoordinator::Instance()->getComponent<Transform>(entity);
                    auto& rigidBody = TheCoordinator::Instance()->getComponent<RigidBody>(entity);
                
                    if (TheInputManager::Instance()->isKeyDown(SDLK_w)) {
                        transform.y -= (10.0f * deltaTime);
                    }
                    if (TheInputManager::Instance()->isKeyDown(SDLK_a)) {
                        transform.x -=  (10.0f * deltaTime);
                    }
                    if (TheInputManager::Instance()->isKeyDown(SDLK_s)) {
                        transform.y +=  (10.0f * deltaTime);
                    }
                    if (TheInputManager::Instance()->isKeyDown(SDLK_d)) {
                        transform.x +=  (10.0f * deltaTime);
                    }  
                }
            }
    };
}

#endif // __InputSystem__