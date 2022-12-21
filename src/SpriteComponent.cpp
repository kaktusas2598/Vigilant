#include "SpriteComponent.hpp"

#include "Entity.hpp"
#include "TextureManager.hpp"

namespace Vigilant {

    SpriteComponent::SpriteComponent(Entity* owner) : Component(owner) {
        isAnimated = false;
        isFlipped = false;
        isAbsolute = false;
        animationState = STILL;
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
        TheTextureManager::Instance()->draw(
                textureID,
                owner->transform->getX(),
                owner->transform->getY(),
                width,
                height,
                flip,
                owner->transform->getScaleX(),
                owner->transform->getScaleY(),
                isAbsolute
                );

    }

    void SpriteComponent::drawFrame(SDL_RendererFlip flip) {
        // FIXME: for some reason SpriteComponent::update() is called after render(), therefore had to calculate frame here
        currentFrame = int(((SDL_GetTicks() / 100) % animationList[currentAnimation].numFrames));

        // Check if once shot animation has finished
        if (animationState == ONE_SHOT && currentFrame == animationList[currentAnimation].numFrames)
        	animationState = STILL;

        TheTextureManager::Instance()->drawFrame(
                textureID,
                owner->transform->getX(),
                owner->transform->getY(),
                width,
                height,
                animationList[currentAnimation].row,
                currentFrame,
                flip,
                owner->transform->getScaleX(),
                owner->transform->getScaleY(),
                isAbsolute
                );
    }
}
