#include "InputComponent.hpp"

#include "InputManager.hpp"
#include "SoundManager.hpp"
#include "Entity.hpp"

#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "ProjectileComponent.hpp"

#include "EntityManager.hpp"

namespace Vigilant {

    InputComponent::InputComponent(Entity* owner) : Component(owner) {}

    void InputComponent::update(float deltaTime) {
                owner->getComponent<SpriteComponent>()->setAnimated(false);
                owner->getComponent<PhysicsComponent>()->unsetForce();

                if (TheInputManager::Instance()->isKeyDown(SDLK_w)) {
                    owner->getComponent<PhysicsComponent>()->applyForceY(-10.0f);
                    owner->getComponent<SpriteComponent>()->setAnimation("up");
                }
                if (TheInputManager::Instance()->isKeyDown(SDLK_a)) {
                    owner->getComponent<PhysicsComponent>()->applyForceX(-10.0f);
                    owner->getComponent<SpriteComponent>()->setAnimation("default");
                    owner->getComponent<SpriteComponent>()->setFlipped(true);
                }
                if (TheInputManager::Instance()->isKeyDown(SDLK_s)) {
                    owner->getComponent<PhysicsComponent>()->applyForceY(10.0f);
                    owner->getComponent<SpriteComponent>()->setAnimation("down");
                    owner->getComponent<SpriteComponent>()->setAnimated(true);
                }
                if (TheInputManager::Instance()->isKeyDown(SDLK_d)) {
                    owner->getComponent<PhysicsComponent>()->applyForceX(10.0f);
                    owner->getComponent<SpriteComponent>()->setAnimation("default");
                    owner->getComponent<SpriteComponent>()->setFlipped(false);
                }
                if (TheInputManager::Instance()->isKeyPressed(SDLK_SPACE)) {
                    TheSoundManager::Instance()->playSound("jump", 0);
                    owner->getComponent<PhysicsComponent>()->setVelocityY( owner->getComponent<PhysicsComponent>()->getVelocityY() - 5.0f);
                }
                if (TheInputManager::Instance()->isKeyPressed(SDLK_f)) {
                    //Entity* entity = new Entity();
                    Entity* entity = EntityManager::Instance()->addEntity();
                    entity->addComponent<SpriteComponent>();
                    entity->addComponent<ColliderComponent>();
                    entity->addComponent<ProjectileComponent>();

					entity->getComponent<SpriteComponent>()->load("bullet", 10, 10);
                    entity->getComponent<ColliderComponent>()->load("projectile", 10, 10);

					int speed = 150;
					int range = 1000;
					Vector2D velocity = owner->getComponent<PhysicsComponent>()->getVelocity();
					velocity.normalize();
					velocity *= speed;
					entity->getComponent<ProjectileComponent>()->load(owner, range, speed, velocity);
                }
                // Alternative movement - follow mouse
                // Vector2D target = TheInputManager::Instance()->getMouseCoords();
                // m_velocity = target - m_position;
                // m_velocity /= 100;

            }
}
