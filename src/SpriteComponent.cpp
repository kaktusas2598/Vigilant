#include "SpriteComponent.hpp"

#include "Entity.hpp"
#include "Engine.hpp"
#include "TextureManager.hpp"

namespace Vigilant {

    SpriteComponent::SpriteComponent(Entity* owner) : Component(owner) {
        isAnimated = false;
        isFlipped = false;
    }

    void SpriteComponent::render() {
        // Move flip stuff to load method
        SDL_RendererFlip flip;
        if (isFlipped) {
            flip = SDL_FLIP_HORIZONTAL;
        } else {
            flip = SDL_FLIP_NONE;
        }
        
        if (isAnimated) {
            drawFrame(flip);
        } else {
            draw(flip);
        }
        
    }

    void SpriteComponent::update(float deltaTime) {
        if (isAnimated) {
            int numFrames = animationList[currentAnimation].numFrames;
            currentFrame =  int(((SDL_GetTicks() / 100) % numFrames)); //int(((SDL_GetTicks() / 100) % 6));
        }
    }

    void SpriteComponent::draw(SDL_RendererFlip flip) {
        SDL_Rect srcRect;
        SDL_Rect destRect;

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = width;
        srcRect.h = height;
        destRect.w = width * owner->transform->getScaleX();
        destRect.h = height * owner->transform->getScaleY();
        destRect.x = owner->transform->getX();// - TheEngine::Instance()->camera.x;
        destRect.y = owner->transform->getY();// - TheEngine::Instance()->camera.y;

        SDL_RenderCopyEx(
            TheEngine::Instance()->getSDLRenderer(),
            TheTextureManager::Instance()->getTexture(textureID),
            &srcRect,
            &destRect,
            0,
            0,
            flip
        );
    }

    void SpriteComponent::drawFrame(SDL_RendererFlip flip) {
        SDL_Rect srcRect;
        SDL_Rect destRect;

        // int currentFrame = int(((SDL_GetTicks() / 100) % 6));
        
        srcRect.x = width * currentFrame;
        srcRect.y = height * (animationList[currentAnimation].row - 1);
        srcRect.w = width;
        srcRect.h = height;
        destRect.w = width * owner->transform->getScaleX();
        destRect.h = height * owner->transform->getScaleY();
        destRect.x = owner->transform->getX();// - TheEngine::Instance()->camera.x;
        destRect.y = owner->transform->getY();// - TheEngine::Instance()->camera.y;

        SDL_RenderCopyEx(
            TheEngine::Instance()->getSDLRenderer(),
            TheTextureManager::Instance()->getTexture(textureID),
            &srcRect,
            &destRect,
            0,
            0,
            flip
        );
    }
}