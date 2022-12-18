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

            void load(int width, int height);
            void update(float deltaTime);
            void render();

            SDL_Rect& getCollider() { return collider; }
            void setCollider(SDL_Rect col) { collider = col; }
        private:
            SDL_Rect collider;
            // Collider box offsets from sprite texture
            int xOffset, yOffset;
    };
}

#endif // __ColliderComponent__
