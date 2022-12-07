#include "ProjectileComponent.hpp"

#include "Entity.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "Engine.hpp"

namespace Vigilant {

    ProjectileComponent::ProjectileComponent(Entity* owner) : Component(owner) { }

	void ProjectileComponent::load(Entity* e, int rng, int sp, Vector2D vel) {
        distance = 0;
        shooter = e;
        range = rng;
        speed = sp;
        velocity = vel;

        float x = shooter->transform->getX() + shooter->getComponent<SpriteComponent>()->getWidth()/2;
        float y = shooter->transform->getY() + shooter->getComponent<SpriteComponent>()->getHeight()/2;
        owner->transform->setX(x);
        owner->transform->setY(y);
    }

    void ProjectileComponent::update(float deltaTime) {
        Vector2D position(owner->transform->getX(), owner->transform->getY());

        position += velocity * deltaTime;

        owner->transform->setX(position.getX());
        owner->transform->setY(position.getY());


        distance += 5; //speed;

        if (distance > range)  {
            owner->destroy();
        }

        if(position.getX() < TheEngine::Instance()->camera.x || position.getX() > TheEngine::Instance()->camera.x + TheEngine::Instance()->camera.w ||
            position.getY() < TheEngine::Instance()->camera.y || position.getY() > TheEngine::Instance()->camera.y + TheEngine::Instance()->camera.h) {
            owner->destroy();
        }
    }

}
