#include "PhysicsComponent.hpp"

#include "Entity.hpp"

namespace Vigilant {

    PhysicsComponent::PhysicsComponent(Entity* owner) : Component(owner) {
		mass = 1.0;
    }

    void PhysicsComponent::preUpdate(float deltaTime) {
        previousPosition.setX(owner->transform->getX());
        previousPosition.setY(owner->transform->getY());
	}

    void PhysicsComponent::update(float deltaTime) {
        Vector2D position(owner->transform->getX(), owner->transform->getY());

		// Friction is a force opposite velocity
		friction = velocity;
		friction *= -1.0f;
		friction *= frictionCoefficient;

		// 2nd Newton Law: F = m * a
        acceleration.setX((force.getX() + friction.getX() + gravity.getX())/mass);
        acceleration.setY((force.getY() + friction.getY() + gravity.getY())/mass);

        velocity += acceleration * deltaTime;
        position += velocity * deltaTime;

        owner->transform->setX(position.getX());
        owner->transform->setY(position.getY());

        // 2nd Newton Law for Rigid Body
        // Forces + Friction = mass * acceleration
        //
        // accX = (force.x + friction.x) / mass
        // Same for accY, but friction can be gravity if it;s platformer
        //
        // I will apply both friction and gravity at the same step because they are both vectors
        //
        // velocity = acceleration * dt
        // position = velocity * dt
        //
        // So in InputComponent instead of changing player position we will apply
        // force instead!!
    }
}
