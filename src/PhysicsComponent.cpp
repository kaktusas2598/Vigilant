#include "PhysicsComponent.hpp"

#include "Entity.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

    PhysicsComponent::PhysicsComponent(Entity* owner) : Component(owner) {}
    
    void PhysicsComponent::update(float deltaTime) {
        Vector2D position(owner->transform->getX(), owner->transform->getY());
        Vector2D velocity(velocityX, velocityY);
        Vector2D acceleration(accelerationX, accelerationY);
        Vector2D gravityForce(gravityX, gravityY);

        position += velocity * deltaTime;
        velocity += gravityForce * deltaTime;

        owner->transform->setX(position.getX());
        owner->transform->setY(position.getY());
        velocityX = velocity.getX();
        velocityY = velocity.getY();       
    }
}