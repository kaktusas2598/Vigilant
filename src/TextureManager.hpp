#ifndef __TextureManager__
#define __TextureManager__

#include <string>
#include <map>
#include <SDL2/SDL.h>

namespace Vigilant {

    /**
     * Alternative to GLTextures for now, texture loading and drawing is subject to change
     * 
     * 
     */
    class TextureManager
    {
    public:

        bool load(std::string fileName, std::string id);
        void draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void setRenderer(SDL_Renderer *rnd) { renderer = rnd; }

        // This class is a singleton
        static TextureManager* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new TextureManager();
                return s_pInstance;
            }
            return s_pInstance;
        }

    private:

        TextureManager() {}

        std::map<std::string, SDL_Texture*> m_textureMap;
        static TextureManager* s_pInstance;
        // Optional for SDL Rendering
        SDL_Renderer *renderer;
        
    };

    typedef TextureManager TheTextureManager;
}

#endif // __TextureManager__