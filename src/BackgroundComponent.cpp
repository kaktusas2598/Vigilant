#include "BackgroundComponent.hpp"

#include "Entity.hpp"

#include "TextureManager.hpp"
#include "Engine.hpp"

namespace Vigilant {

    BackgroundComponent::BackgroundComponent(Entity* owner): Component(owner) {
        count = 0;
        maxCount = 10;
    }

    // Must be loaded after Sprite component!
    void BackgroundComponent::load(int scrollSp) {
        scrollSpeed = scrollSp;
        sprite = owner->getComponent<SpriteComponent>();

        srcRect1.x = 0;
        destRect1.x = owner->transform->getX();
        srcRect1.y = 0;
        destRect1.y = owner->transform->getY();

        srcRect1.w = sprite->getWidth();
        destRect1.w = TheEngine::Instance()->getScreenWidth();
        srcRect1.h = sprite->getHeight();
        destRect1.h = TheEngine::Instance()->getScreenHeight();

        srcRect2.x = 0;
        destRect2.x = owner->transform->getX() + TheEngine::Instance()->getScreenWidth();
        srcRect2.y = 0;
        destRect2.y = owner->transform->getY();

        srcRect2.w = destRect2.w = 0;
        srcRect2.h = sprite->getHeight();
        destRect2.h = TheEngine::Instance()->getScreenHeight();

    }

    void BackgroundComponent::render() {
        // draw first rect
        TheTextureManager::Instance()->draw(sprite->getTextureID(), srcRect1, destRect1);

        // draw second rect
        TheTextureManager::Instance()->draw(sprite->getTextureID(), srcRect2, destRect2);
    }

    void BackgroundComponent::update(float deltaTime) {
        if (count == maxCount) {
            //make first rectangle smaller
            srcRect1.x += scrollSpeed;
            srcRect1.w -= scrollSpeed;
            destRect1.w -= scrollSpeed;

            //make second rectangle bigger
            srcRect2.w += scrollSpeed;
            destRect2.w += scrollSpeed;
            destRect2.x -= scrollSpeed;

            // reset rectangles
            if (destRect2.w >= TheEngine::Instance()->getScreenWidth()) {
                srcRect1.x = 0;
                destRect1.x = owner->transform->getX();
                srcRect1.y = 0;
                destRect1.y = owner->transform->getY();

                srcRect1.w = sprite->getWidth();
                destRect1.w = TheEngine::Instance()->getScreenWidth();
                srcRect1.h = sprite->getHeight();
                destRect1.h = TheEngine::Instance()->getScreenHeight();

                srcRect2.x = 0;
                destRect2.x = owner->transform->getX() + TheEngine::Instance()->getScreenWidth();
                srcRect2.y = 0;
                destRect2.y = owner->transform->getY();

                srcRect2.w = destRect2.w = 0;
                srcRect2.h = sprite->getHeight();
                destRect2.h = TheEngine::Instance()->getScreenHeight();
            }
            count = 0;
        }
        count++;
    }
}
