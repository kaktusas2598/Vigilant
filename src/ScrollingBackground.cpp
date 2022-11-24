#include "ScrollingBackground.hpp"
#include "TextureManager.hpp"
#include "Engine.hpp"

namespace Vigilant {

    ScrollingBackground::ScrollingBackground(): SDLEntity() {
        count = 0;
        maxCount = 10;
    }

    void ScrollingBackground::load(const LoaderParams* params) {
        SDLEntity::load(params);

        // scrollSpeed = pParams->getAnimSpeed();
    
        scrollSpeed = 1;
        
        srcRect1.x = 0;
        destRect1.x = m_position.getX();
        srcRect1.y = 0;
        destRect1.y = m_position.getY();
        
        srcRect1.w = destRect1.w = srcRect2Width = destRect1Width = m_width;
        srcRect1.h = destRect1.h = m_height;
        
        srcRect2.x = 0;
        destRect2.x = m_position.getX() + m_width;
        srcRect2.y = 0;
        destRect2.y = m_position.getY();
        
        srcRect2.w = destRect2.w = srcRect2Width = destRect2Width = 0;
        srcRect2.h = destRect2.h = m_height;
    }

    // FIXME: Adding scrolling background while laptop is on battery seems to have dramatic performance impact
    // So consider either removing this altogether or making this better
    void ScrollingBackground::draw(float deltaTime) {
        // SDLEntity::draw(deltaTime);
        // draw first rect
        SDL_RenderCopyEx(TheEngine::Instance()->getSDLRenderer(), TheTextureManager::Instance()->getTextureMap()[m_textureID], &srcRect1, &destRect1, 0, 0, SDL_FLIP_NONE);
        
        // draw second rect
        SDL_RenderCopyEx(TheEngine::Instance()->getSDLRenderer(), TheTextureManager::Instance()->getTextureMap()[m_textureID], &srcRect2, &destRect2, 0, 0, SDL_FLIP_NONE);
    }

    void ScrollingBackground::update(float deltaTime) {
        if (count == maxCount) {
            //make first rectangle smaller/
            srcRect1.x += scrollSpeed;
            srcRect1.w -= scrollSpeed;
            destRect1.w -= scrollSpeed;

            //make second rectangle bigger
            srcRect2.w += scrollSpeed;
            destRect2.w += scrollSpeed;
            destRect2.x -= scrollSpeed;

            // reset rectangles
            if (destRect2.w >= m_width) {
                srcRect1.x = 0;
                destRect1.x = m_position.getX();
                srcRect1.y = 0;
                destRect1.y = m_position.getY();

                srcRect1.w = destRect1.w = srcRect2Width = destRect2Width = m_width;
                srcRect1.h = destRect1.h = m_height;
                
                srcRect2.x = 0;
                destRect2.x = m_position.getX() + m_width;
                srcRect2.y = 0;
                destRect2.y = m_position.getY();
                
                srcRect2.w = destRect2.w = srcRect2Width = destRect2Width = 0;
                srcRect2.h = destRect2.h = m_height;
            }
            count = 0;
        }
        count++;
    }

    void ScrollingBackground::clean() {
        SDLEntity::clean();
    }
}