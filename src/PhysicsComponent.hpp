#ifndef __PhysicsComponent__
#define __PhysicsComponent__

#include "Component.hpp"

namespace Vigilant {

    class PhysicsComponent : public Component {
        public:
            PhysicsComponent(Entity* owner);
            void update(float deltaTime);

            void load(float velX, float velY, float accX, float accY, float gX, float gY) {
                velocityX = velX;
                velocityY = velY;
                accelerationX = accX;
                accelerationY = accY;
                gravityX = gX;
                gravityY = gY;
            }

            float getVelocityX() const { return velocityX; }
            float getVelocityY() const { return velocityY; }
            float getAccelerationX() const { return accelerationX; }
            float getAccelerationY() const { return accelerationY; }
            float getGravityX() const { return gravityX; }
            float getGravityY() const { return gravityY; }

            void setVelocityX(float X) { velocityX = X; }
            void setVelocityY(float Y) { velocityY = Y; }
            void setAccelerationX(float X) { accelerationX = X; }
            void setAccelerationY(float Y) { accelerationY = Y; }
            void setGravityX(float X) { gravityY = X; }
            void setGravityY(float Y) { gravityY = Y; }
        private:
            float velocityX;
            float velocityY;
            float accelerationX;
            float accelerationY;
            float gravityX;
            float gravityY;
    };
}

#endif // __PhysicsComponent__