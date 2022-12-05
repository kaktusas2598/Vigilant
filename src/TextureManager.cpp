#include "TextureManager.hpp"
#include <SDL2/SDL_image.h>

#include "Engine.hpp"

namespace Vigilant {

    TextureManager* TextureManager::s_pInstance = 0;

    bool TextureManager::load(std::string fileName, std::string id) {
        SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

        if(pTempSurface == 0)
        {
            return false;
        }

        SDL_Texture* pTexture = SDL_CreateTextureFromSurface(renderer, pTempSurface);

        SDL_FreeSurface(pTempSurface);

        if(pTexture != 0)
        {
            m_textureMap[id] = pTexture;
            return true;
        }

        return false;
    }

    void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip) {
        SDL_Rect srcRect;
        SDL_Rect destRect;

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = destRect.w = width;
        srcRect.h = destRect.h = height;
        destRect.x = x;
        destRect.y = y;

        SDL_RenderCopyEx(renderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
    }

    void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_RendererFlip flip) {
        SDL_Rect srcRect;
        SDL_Rect destRect;
        srcRect.x = width * currentFrame;
        srcRect.y = height * (currentRow - 1);
        srcRect.w = destRect.w = width;
        srcRect.h = destRect.h = height;
        destRect.x = x;
        destRect.y = y;

        SDL_RenderCopyEx(renderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
    }

    void TextureManager::drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, double scale) {
        SDL_Rect srcRect;
        SDL_Rect destRect;
        srcRect.x = margin + (spacing + width) * currentFrame;
        srcRect.y = margin + (spacing + height) * currentRow;
        srcRect.w = width;
        srcRect.h = height;
        destRect.w = width * scale;
        destRect.h = height * scale;
        destRect.x = x; //- TheEngine::Instance()->camera.x;
        destRect.y = y; //- TheEngine::Instance()->camera.y;

        SDL_RenderCopyEx(renderer, m_textureMap[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
    }

    void TextureManager::clearTextureMap() {
        m_textureMap.clear();
    }

    void TextureManager::clearFromTextureMap(std::string id) {
        m_textureMap.erase(id);
    }
}
