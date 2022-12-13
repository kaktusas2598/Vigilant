#ifndef __ColliderComponent__
#define __ColliderComponent__

#include "Component.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Vigilant {

    class Entity;

    class ColliderComponent : public Component {
        public:
            ColliderComponent(Entity* owner);

            void load(std::string tag, int width, int height) {
                type = tag;
                collider.w = width;
                collider.h = height;
            }

            void update(float deltaTime);
            void render() {}

            SDL_Rect& getCollider() { return collider; }
            void setCollider(SDL_Rect col) { collider = col; }

            void setListener(std::string listener) { listenerName = listener; }
            std::string getListener() { return listenerName; }
        private:
            SDL_Rect collider;
            std::string type;

			// Name of Lua function which will act as onCollide listener
			// Colliding Entity will be passed to this
			std::string listenerName;
    };
}

#endif // __ColliderComponent__
