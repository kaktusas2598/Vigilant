#ifndef __PhysicsSystem__
#define __PhysicsSystem__

#include "ECS.hpp"
#include "Coordinator.hpp"
#include "ComponentTypes.hpp"

namespace Vigilant {

    class PhysicsSystem : public System {
        public:
            void update(float deltaTime) {
                for (auto const& entity : entities) {
                    auto& rigidBody = TheCoordinator::Instance()->getComponent<RigidBody>(entity);
                    auto& transform = TheCoordinator::Instance()->getComponent<Transform>(entity);
                    auto const& gravity = TheCoordinator::Instance()->getComponent<Gravity>(entity);

                    Vector2D position(transform.x, transform.y);
                    Vector2D velocity(rigidBody.velocityX, rigidBody.velocityY);
                    Vector2D gravityForce(gravity.forceX, gravity.forceY);

                    position += velocity * deltaTime;
                    velocity += gravityForce * deltaTime;

                    transform.x = position.getX();
                    transform.y = position.getY();
                    rigidBody.velocityX = velocity.getX();
                    rigidBody.velocityY = velocity.getY();

                    
                    // auto const& playable = TheCoordinator::Instance()->getComponent<Playable>(entity);
                    // int test;
                    // if (playable != NULL) {
                    //     // TODO check if jump started
                    // }
                }
            }
    };
}

#endif // __PhysicsSystem__