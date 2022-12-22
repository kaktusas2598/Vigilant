#ifndef __BackgroundComponent__
#define __BackgroundComponent__

#include "Component.hpp"
#include "SpriteComponent.hpp"

namespace Vigilant {

    class BackgroundComponent : public Component {
        public:
            BackgroundComponent(Entity* owner);

            // Default backgroun will be static, if scroll speed not 0, it will scroll
            void load(int scrollSp = 0);

            void reload() {
                load(scrollSpeed);
            }

            void render();
            void update(float deltaTime);

        private:
            int scrollSpeed;

            // ??
            int count;
            int maxCount;

            SDL_Rect srcRect1;
            SDL_Rect srcRect2;

            SDL_Rect destRect1;
            SDL_Rect destRect2;

            // Store pointer to sprite so we don't have to request it every time, because width and height should not change
            SpriteComponent* sprite;
    };
}

#endif // __BackgroundComponent__
