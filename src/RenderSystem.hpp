#ifndef __RenderingSystem__
#define __RenderingSystem__

#include "ECS.hpp"
#include "Coordinator.hpp"
#include "ComponentTypes.hpp"

#include <SDL2/SDL.h>

//TEMP
#include "IEngine.hpp"
#include "TextureManager.hpp"

namespace Vigilant {

    class RenderSystem : public System {
        public:
            void render() {
                for(auto const& entity: entities) {
                    auto& transform = TheCoordinator::Instance()->getComponent<Transform>(entity);
                    auto& sprite = TheCoordinator::Instance()->getComponent<Sprite>(entity);
                    SDL_Rect srcRect;
                    SDL_Rect destRect;

                    srcRect.x = 0;
                    srcRect.y = 0;
                    srcRect.w = destRect.w = sprite.width;
                    srcRect.h = destRect.h = sprite.height;
                    destRect.x = transform.x;
                    destRect.y = transform.y;

                    //TODO:
                    SDL_RendererFlip flip = SDL_FLIP_NONE;

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
            }

            void renderFrame() {}
            // void renderTile() {}
    };
}

#endif // __RenderingSystem__