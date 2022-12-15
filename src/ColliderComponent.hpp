#ifndef __ColliderComponent__
#define __ColliderComponent__

#include "Component.hpp"
#include "LuaListener.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Vigilant {

    class Entity;

    class ColliderComponent : public Component, public LuaListener {
        public:
            ColliderComponent(Entity* owner);

            void load(std::string tag, int width, int height) {
                type = tag;
                collider.w = width;
                collider.h = height;
                collider.x = 0;
                collider.y = 0;
            }

            void update(float deltaTime);
            void render() {}

            SDL_Rect& getCollider() { return collider; }
            void setCollider(SDL_Rect col) { collider = col; }

        private:
            SDL_Rect collider;
            std::string type;
            // Collider box offsets from sprite texture
            int xOffset, yOffset;
    };
}

#endif // __ColliderComponent__
