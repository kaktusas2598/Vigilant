#ifndef __InputSystem__
#define __InputSystem__

#include "ECS.hpp"
#include "Coordinator.hpp"
#include "ComponentTypes.hpp"
#include "InputManager.hpp"
#include "SoundManager.hpp"

namespace Vigilant {

    class InputSystem : public System {
        public:
            void update(float deltaTime) {
                for (auto const& entity : entities) {
                    auto& sprite = TheCoordinator::Instance()->getComponent<Sprite>(entity);
                    auto& transform = TheCoordinator::Instance()->getComponent<Transform>(entity);
                    auto& rigidBody = TheCoordinator::Instance()->getComponent<RigidBody>(entity);
                
                    if (TheInputManager::Instance()->isKeyDown(SDLK_w)) {
                        transform.y -= (10.0f * deltaTime);
                    }
                    if (TheInputManager::Instance()->isKeyDown(SDLK_a)) {
                        transform.x -=  (10.0f * deltaTime);
                        sprite.flipped = true;
                    }
                    if (TheInputManager::Instance()->isKeyDown(SDLK_s)) {
                        transform.y +=  (10.0f * deltaTime);
                    }
                    if (TheInputManager::Instance()->isKeyDown(SDLK_d)) {
                        transform.x +=  (10.0f * deltaTime);
                        sprite.flipped = false;
                    }
                    if (TheInputManager::Instance()->isKeyPressed(SDLK_SPACE)) {
                        TheSoundManager::Instance()->playSound("jump", 0);
                        rigidBody.velocityY = -20.0f;
                        // transform.x +=  (10.0f * deltaTime);
                    }
                    // Alternative movement - follow mouse
                    // Vector2D target = TheInputManager::Instance()->getMouseCoords();
                    // m_velocity = target - m_position;
                    // m_velocity /= 100;
                }
            }
    };
}

#endif // __InputSystem__