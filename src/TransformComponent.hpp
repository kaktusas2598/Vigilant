#ifndef __TransformComponent__
#define __TransformComponent__

#include "Component.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

    class TransformComponent : public Component {
        public:
            TransformComponent(Entity* owner) : Component(owner) {
                x = 0;
                y = 0;
                scaleX = 1.0f;
                scaleY = 1.0f;
            }

            float getX() const { return x; }
            float getY() const { return y; }
            float getScaleX() const { return scaleX; }
            float getScaleY() const { return scaleY; }

            void setX(float X) { x = X; }
            void setY(float Y) { y = Y; }
            void setScaleX(float scale) { scaleX = scale; }
            void setScaleY(float scale) { scaleY = scale; }
        private:
            float x;
            float y;
            float scaleX;
            float scaleY;
    };
}

#endif // __TransformComponent__