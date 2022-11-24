#ifndef __RenderingSystem__
#define __RenderingSystem__

#include "ECS.hpp"
#include "Coordinator.hpp"
#include "ComponentTypes.hpp"

#include <SDL2/SDL.h>

//TEMP
#include "Engine.hpp"
#include "TextureManager.hpp"

namespace Vigilant {

    class RenderSystem : public System {
        public:
            void render() {
                for(auto const& entity: entities) {
                    auto& transform = TheCoordinator::Instance()->getComponent<Transform>(entity);
                    auto& sprite = TheCoordinator::Instance()->getComponent<Sprite>(entity);
                    SDL_RendererFlip flip;

                    if (sprite.flipped) {
                        flip = SDL_FLIP_HORIZONTAL;
                    } else {
                        flip = SDL_FLIP_NONE;
                    }
                    
                    if (sprite.isAnimated) {
                        drawFrame(sprite, transform, flip);
                    } else {
                        draw(sprite, transform, flip);
                    }
                }
            }
        private:
            void draw(Sprite sprite, Transform transform, SDL_RendererFlip flip) {
                SDL_Rect srcRect;
                SDL_Rect destRect;

                srcRect.x = 0;
                srcRect.y = 0;
                srcRect.w = destRect.w = sprite.width;
                srcRect.h = destRect.h = sprite.height;
                destRect.x = transform.x;
                destRect.y = transform.y;

                 SDL_RenderCopyEx(
                    TheEngine::Instance()->getSDLRenderer(),
                    TheTextureManager::Instance()->getTexture(sprite.textureID),
                    &srcRect,
                    &destRect,
                    0,
                    0,
                    flip
                );
            }

            void drawFrame(Sprite sprite, Transform transform, SDL_RendererFlip flip) {
                SDL_Rect srcRect;
                SDL_Rect destRect;

                int currentFrame = int(((SDL_GetTicks() / 100) % 6));

                srcRect.x = sprite.width * currentFrame;
                srcRect.y = sprite.height * (sprite.currentRow - 1);
                srcRect.w = destRect.w = sprite.width;
                srcRect.h = destRect.h = sprite.height;
                destRect.x = transform.x;
                destRect.y = transform.y;

                SDL_RenderCopyEx(
                    TheEngine::Instance()->getSDLRenderer(),
                    TheTextureManager::Instance()->getTexture(sprite.textureID),
                    &srcRect,
                    &destRect,
                    0,
                    0,
                    flip
                );
            }
    };
}

#endif // __RenderingSystem__