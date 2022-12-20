#include "InputComponent.hpp"

#include "InputManager.hpp"
#include "Entity.hpp"

#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"

namespace Vigilant {

    InputComponent::InputComponent(Entity* owner) : Component(owner) {}

    void InputComponent::update(float deltaTime) {
        if (owner->getComponent<SpriteComponent>()->getAnimationState() != ONE_SHOT) {
            owner->getComponent<SpriteComponent>()->setAnimated(false);
        }
        owner->getComponent<PhysicsComponent>()->unsetForce();

        if (TheInputManager::Instance()->isKeyDown(SDLK_w)) {
            owner->getComponent<PhysicsComponent>()->applyForceY(-10.0f);
            owner->getComponent<SpriteComponent>()->playAnimation("up");
        }
        if (TheInputManager::Instance()->isKeyDown(SDLK_a)) {
            owner->getComponent<PhysicsComponent>()->applyForceX(-10.0f);
            owner->getComponent<SpriteComponent>()->playAnimation("default");
            owner->getComponent<SpriteComponent>()->setFlipped(true);
        }
        if (TheInputManager::Instance()->isKeyDown(SDLK_s)) {
            owner->getComponent<PhysicsComponent>()->applyForceY(10.0f);
            owner->getComponent<SpriteComponent>()->playAnimation("down");
        }
        if (TheInputManager::Instance()->isKeyDown(SDLK_d)) {
            owner->getComponent<PhysicsComponent>()->applyForceX(10.0f);
            owner->getComponent<SpriteComponent>()->playAnimation("default");
            owner->getComponent<SpriteComponent>()->setFlipped(false);
        }
        if (TheInputManager::Instance()->isKeyPressed(SDLK_SPACE)) {
            owner->getComponent<PhysicsComponent>()->setVelocityY( owner->getComponent<PhysicsComponent>()->getVelocityY() - 5.0f);
        }
        // Alternative movement - follow mouse
        // Vector2D target = TheInputManager::Instance()->getMouseCoords();
        // m_velocity = target - m_position;
        // m_velocity /= 100;

    }
}
