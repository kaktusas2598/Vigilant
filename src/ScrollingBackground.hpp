#ifndef __ScrollingBackground__
#define __ScrollingBackground__

#include "SDLEntity.hpp"
#include "EntityFactory.hpp"
#include <SDL2/SDL.h>

namespace Vigilant {
    
    class ScrollingBackground : public SDLEntity {
        public:
            ScrollingBackground();

			void draw(float deltaTime);
			void update(float deltaTime);
			void clean();

			void load(const LoaderParams *params);
        private:
            int scrollSpeed;

            // ??
            int count;
            int maxCount;

            SDL_Rect srcRect1;
            SDL_Rect srcRect2;

            SDL_Rect destRect1;
            SDL_Rect destRect2;

            int srcRect1Width;
            int srcRect2Width;

            int destRect1Width;
            int destRect2Width;
    };

    class ScrollingBackgroundCreator : public BaseCreator {
        public:
            virtual IEntity* createEntity() const {
                return new ScrollingBackground();
            }
    };
}


#endif // _ScrollingBackground__