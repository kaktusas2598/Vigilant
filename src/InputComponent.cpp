#include "InputComponent.hpp"

#include "InputManager.hpp"
#include "SoundManager.hpp"
#include "Entity.hpp"

#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"

namespace Vigilant {

    InputComponent::InputComponent(Entity* owner) : Component(owner) {}

    void InputComponent::update(float deltaTime) {
                float x = owner->transform->getX();
                float y = owner->transform->getY();
                owner->getComponent<SpriteComponent>()->setAnimated(false);
                if (TheInputManager::Instance()->isKeyDown(SDLK_w)) {
                    owner->transform->setY(y - (10.0f * deltaTime));
                    // owner->getComponent<PhysicsComponent>()->setVelocityY(-10.0f * deltaTime);
                    owner->getComponent<SpriteComponent>()->setAnimation("up");
                }
                if (TheInputManager::Instance()->isKeyDown(SDLK_a)) {
                    owner->transform->setX(x - (10.0f * deltaTime));
                    // owner->getComponent<PhysicsComponent>()->setVelocityX(-10.0f * deltaTime);
                    owner->getComponent<SpriteComponent>()->setAnimation("default");
                    owner->getComponent<SpriteComponent>()->setFlipped(true);
                }
                if (TheInputManager::Instance()->isKeyDown(SDLK_s)) {
                    owner->getComponent<SpriteComponent>()->setAnimation("down");
                    owner->transform->setY(y + (10.0f * deltaTime));
                    // owner->getComponent<PhysicsComponent>()->setVelocityY(10.0f * deltaTime);
                    owner->getComponent<SpriteComponent>()->setAnimated(true);
                }
                if (TheInputManager::Instance()->isKeyDown(SDLK_d)) {
                    owner->transform->setX(x + (10.0f * deltaTime));
                    // owner->getComponent<PhysicsComponent>()->setVelocityX(10.0f * deltaTime);
                    owner->getComponent<SpriteComponent>()->setAnimation("default");
                    owner->getComponent<SpriteComponent>()->setFlipped(false);
                }
                if (TheInputManager::Instance()->isKeyPressed(SDLK_SPACE)) {
                    TheSoundManager::Instance()->playSound("jump", 0);
                    owner->getComponent<PhysicsComponent>()->setVelocityY( owner->getComponent<PhysicsComponent>()->getVelocityY() - 5.0f);
                }
                // Alternative movement - follow mouse
                // Vector2D target = TheInputManager::Instance()->getMouseCoords();
                // m_velocity = target - m_position;
                // m_velocity /= 100;

            }
}
