#ifndef __TextureManager__
#define __TextureManager__

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Vigilant {

    /**
     * Alternative to GLTextures for now, texture loading and drawing is subject to change
     *
     * 2022-12-12: Adding TTF Font support, but by now this class already breaks SOLID principle and does 3 things:
     * 1. Loads and manages textures using SDL_Texture*
     * 2. Loads and manages TTF Fonts using SDL2_ttf
     * 3. Does rendering which really needs to have it's own separate system
     *
     */
    class TextureManager {
    public:

        bool load(std::string fileName, std::string id);

        void draw(
                std::string id,
                int x, int y,
                int width, int height,
                SDL_RendererFlip flip = SDL_FLIP_NONE,
                float scaleX = 1.0f,
                float scaleY = 1.0f,
                bool absolutePos = true
                );
        void draw(std::string id, int x, int y, int width, int height, double angle, SDL_RendererFlip flip = SDL_FLIP_NONE);
        /*
         * Render a tile from a spritesheet
         *
         * @param absolutePos if set to true will always stay in the same position on the screen,
         * if false, it will render according to the camera
         */
        void drawFrame(
            std::string id,
            int x, int y,
            int width, int height,
            int currentRow, int currentFrame,
            SDL_RendererFlip flip = SDL_FLIP_NONE,
            float scaleX = 1.0f,
            float scaleY = 1.0f,
            bool absolutePos = true
            );
        void drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, double scale = 1.0f);
        void drawRect(SDL_Rect rect, SDL_Color color, bool absolutePos = true, bool fill = false);

        SDL_Texture* getTextTexture(std::string font, std::string text, SDL_Color color = {255, 255, 255, 255});
        void draw(SDL_Texture* texture, int x, int y);
        void draw(std::string id, SDL_Rect srcRect, SDL_Rect dstRect);

        // These 2 should go to FontManager class OR could refactor this class to AssetManager?
        bool loadFont(std::string fileName, std::string id);
        TTF_Font* getFont(std::string id) { return m_fontMap[id]; }

        SDL_Renderer* getRenderer() { return renderer; }
        void setRenderer(SDL_Renderer *rnd) { renderer = rnd; }

        SDL_Texture* getTexture(std::string id) { return m_textureMap[id]; }

        // This class is a singleton
        static TextureManager* Instance() {
            if(s_pInstance == 0) {
                s_pInstance = new TextureManager();
                return s_pInstance;
            }
            return s_pInstance;
        }

        std::map<std::string, SDL_Texture*> getTextureMap() { return m_textureMap; }
        void clearTextureMap();
        void clearFromTextureMap(std::string id);

        void clearFontMap();
        void clearFromFontMap(std::string id);

    private:

        TextureManager() {}

        std::map<std::string, SDL_Texture*> m_textureMap;
        std::map<std::string, TTF_Font*> m_fontMap;
        static TextureManager* s_pInstance;
        // Optional for SDL Rendering
        SDL_Renderer *renderer;

    };

    typedef TextureManager TheTextureManager;
}

#endif // __TextureManager__
