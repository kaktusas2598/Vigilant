#ifndef __PhysicsComponent__
#define __PhysicsComponent__

#include "Component.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

    class PhysicsComponent : public Component {
        public:
            PhysicsComponent(Entity* owner);
            void update(float deltaTime);

            void load(float velX, float velY, float accX, float accY, float gX, float gY) {
            	velocity.setX(velX);
            	velocity.setY(velY);
            	acceleration.setX(accX);
            	acceleration.setY(accY);
            	gravity.setX(accX);
            	gravity.setY(accY);
            }

            float getMass() const { return mass; }
            void setMass(float m) { mass = m; }

			float getVelocityX() { return velocity.getX(); }
			float getVelocityY() { return velocity.getY(); }
            //float getAccelerationX() const { return accelerationX; }
            //float getAccelerationY() const { return accelerationY; }
            //float getGravityX() const { return gravityX; }
            //float getGravityY() const { return gravityY; }

			//float getForceX() const { return forceX; }
            //float getForceY() const { return forceY; }

            void setVelocityX(float X) { velocity.setX(X); }
            void setVelocityY(float Y) { velocity.setY(Y); }
            void setAccelerationX(float X) { acceleration.setX(X); }
            void setAccelerationY(float Y) { acceleration.setY(Y); }
            void setGravityX(float X) { gravity.setX(X); }
            void setGravityY(float Y) { gravity.setY(Y); }

            void setFriction(float X, float Y) {
				friction.setX(X);
				friction.setY(Y);
            }
            void setFrictionCoefficient(float f) { frictionCoefficient = f; }

            void applyForce(float X, float Y) {
				force.setX(X);
				force.setY(Y);
            }
			void applyForceX(float X) { force.setX(X); }
            void applyForceY(float Y) { force.setY(Y); }
            void unsetForce() {
				force.setX(0);
				force.setY(0);
            }

        private:
            float mass;
			float frictionCoefficient;
			Vector2D velocity;
			Vector2D acceleration;
			Vector2D gravity;
			Vector2D force;
			Vector2D friction;
    };
}

#endif // __PhysicsComponent__
