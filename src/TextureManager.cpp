#include "TextureManager.hpp"
#include <SDL2/SDL_image.h>

#include "Engine.hpp"
#include "Logger.hpp"

namespace Vigilant {

    TextureManager* TextureManager::s_pInstance = 0;

    bool TextureManager::load(std::string fileName, std::string id) {
        // Protect against loading texture with same id for now
        // In the future we just want to make sure we dont double load same id unless we want to change
        // actual texture, this will be used to catch any logical errors for now!
        if (m_textureMap.find(id) != m_textureMap.end()) {
            std::cout << "Trying to load texture with existing id: " << id << std::endl;
            return true;
        }
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


    bool TextureManager::loadFont(std::string fileName, std::string id) {
        if (m_fontMap.find(id) != m_fontMap.end()) {
            return true;
        }

        TTF_Font* font = TTF_OpenFont(fileName.c_str(), 28 );
        if ( font == NULL ) {
            Logger::Instance()->error(TTF_GetError());
            return false;
        }
        m_fontMap[id] = font;
        return true;
    }

    void TextureManager::draw(std::string id,
            int x, int y,
            int width, int height,
            SDL_RendererFlip flip,
            float scaleX, float scaleY,
            bool absolutePos
            ) {
        // All these draw methods do very similar stuff with rectangles,
        // this could be refactored
        SDL_Rect srcRect;
        SDL_Rect destRect;

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = width;
        srcRect.h = height;
        destRect.w = width * scaleX;
        destRect.h = height * scaleY;
        destRect.x = x;
        destRect.y = y;
        destRect.x = x - (absolutePos ? 0 : TheEngine::Instance()->camera.x);
        destRect.y = y - (absolutePos ? 0 : TheEngine::Instance()->camera.y);

        if (SDL_RenderCopyEx(renderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip) != 0) {
            Logger::Instance()->error(SDL_GetError());
        }
    }

    void TextureManager::draw(std::string id, int x, int y, int width, int height, double angle, SDL_RendererFlip flip) {
        SDL_Rect srcRect;
        SDL_Rect destRect;

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = destRect.w = width;
        srcRect.h = destRect.h = height;
        destRect.x = x;
        destRect.y = y;

        if (SDL_RenderCopyEx(renderer, m_textureMap[id], &srcRect, &destRect, angle, 0, flip) != 0) {
            Logger::Instance()->error(SDL_GetError());
        }

    }


    void TextureManager::drawFrame(std::string id,
            int x, int y,
            int width, int height,
            int currentRow, int currentFrame,
            SDL_RendererFlip flip,
            float scaleX, float scaleY,
            bool absolutePos
            ) {
        SDL_Rect srcRect;
        SDL_Rect destRect;
        srcRect.x = width * currentFrame;
        srcRect.y = height * currentRow;
        srcRect.w = width;
        srcRect.h = height;
        destRect.w = width * scaleX;
        destRect.h = height * scaleY;
        destRect.x = x - (absolutePos ? 0 : TheEngine::Instance()->camera.x);
        destRect.y = y - (absolutePos ? 0 : TheEngine::Instance()->camera.y);

        if (SDL_RenderCopyEx(renderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip) != 0) {
            Logger::Instance()->error(SDL_GetError());
        }
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

    void TextureManager::drawRect(SDL_Rect rect, SDL_Color color, bool absolutePos, bool fill) {
        if (!absolutePos) {
            rect.x -= TheEngine::Instance()->camera.x;
            rect.y -= TheEngine::Instance()->camera.y;
        }

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        if (fill)
            SDL_RenderFillRect(renderer, &rect);
        else
            SDL_RenderDrawRect(renderer, &rect);
    }

    SDL_Texture* TextureManager::getTextTexture(std::string font, std::string text, SDL_Color color) {
        // TODO: read about TTF_RenderText_Blended() as well
        SDL_Surface* textSurface = TTF_RenderText_Solid(getFont(font) , text.c_str(), color);
        if( textSurface == NULL ) {
            Logger::Instance()->error(TTF_GetError());
        }
        //Create texture from surface pixels
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if( texture == NULL ) {
            Logger::Instance()->error(SDL_GetError());
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);

        return texture;
    }

    void TextureManager::draw(SDL_Texture* texture, int x, int y) {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }


    void TextureManager::clearTextureMap() {
        m_textureMap.clear();
    }

    void TextureManager::clearFromTextureMap(std::string id) {
        // Lack of this line was the cause of massive memory leak!
        // Beginning of this class many years ago originated from SDL2 Game Development book and it was
        // full of errors like these, so lesson: always make sure clear any dynamically allocted memory!
        SDL_DestroyTexture(m_textureMap[id]);
        m_textureMap.erase(id);
    }

    void TextureManager::clearFontMap() {
        m_fontMap.clear();
    }

    void TextureManager::clearFromFontMap(std::string id) {
        TTF_CloseFont(m_fontMap[id]);
        m_fontMap.erase(id);
    }

}
