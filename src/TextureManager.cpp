#include "TextureManager.hpp"
#include <SDL2/SDL_image.h>

#include "Engine.hpp"
#include "Logger.hpp"

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


    bool TextureManager::loadFont(std::string fileName, std::string id) {
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), 28 );
		if ( font == NULL ) {
            Logger::Instance()->error(TTF_GetError());
			return false;
		}
		m_fontMap[id] = font;
		return true;
	}

    void TextureManager::drawText(std::string text, std::string fontId, int x, int y, SDL_Color color, int scale) {
    	//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid( m_fontMap[fontId], text.c_str(), color );
		if( textSurface == NULL ) {
            Logger::Instance()->error(TTF_GetError());
		}
		//Create texture from surface pixels
		SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( texture == NULL ) {
            Logger::Instance()->error(SDL_GetError());
		}
		else {
			//Get image dimensions
			int width = textSurface->w;
			int height = textSurface->h;

			// Temporarily register created texture surface and after rendering clear it
			// This is not a good design at all, problem with this is described in header file
			m_textureMap[fontId] = texture;
			draw(fontId, x, y, textSurface->w, textSurface->h, SDL_FLIP_NONE);
			clearFromTextureMap(fontId);
		}

		// Get rid of old surface
		SDL_FreeSurface( textSurface );
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


    void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_RendererFlip flip) {
        SDL_Rect srcRect;
        SDL_Rect destRect;
        srcRect.x = width * currentFrame;
        srcRect.y = height * (currentRow - 1);
        srcRect.w = destRect.w = width;
        srcRect.h = destRect.h = height;
        destRect.x = x;
        destRect.y = y;

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

    void TextureManager::clearTextureMap() {
        m_textureMap.clear();
    }

    void TextureManager::clearFromTextureMap(std::string id) {
        m_textureMap.erase(id);
    }

    void TextureManager::clearFontMap() {
        m_fontMap.clear();
    }

    void TextureManager::clearFromFontMap(std::string id) {
        m_fontMap.erase(id);
    }

}
