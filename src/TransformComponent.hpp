#ifndef __TransformComponent__
#define __TransformComponent__

#include "Component.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

    class TransformComponent : public Component {
        public:
            TransformComponent(Entity* owner) : Component(owner) {
                position = Vector2D(0.0f, 0.0f);
                scaleX = 1.0f;
                scaleY = 1.0f;
            }

            Vector2D& getPosition() { return position; }
            void setPosition(Vector2D pos) { position = pos; }

            float getX() { return position.getX(); }
            float getY() { return position.getY(); }
            float getScaleX() const { return scaleX; }
            float getScaleY() const { return scaleY; }

            void setX(float X) { position.setX(X); }
            void setY(float Y) { position.setY(Y); }
            void setScaleX(float scale) { scaleX = scale; }
            void setScaleY(float scale) { scaleY = scale; }
        private:
            Vector2D position;
            float scaleX;
            float scaleY;
    };
}

#endif // __TransformComponent__
