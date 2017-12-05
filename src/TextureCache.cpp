#include "TextureCache.hpp"
#include "TextureLoader.hpp"

#include <string>
#include <iostream>

namespace Vigilant {

    TextureCache::TextureCache()
    {
        //empty
    }

    TextureCache::~TextureCache()
    {
        //empty
    }

    GLTexture TextureCache::getTexture(std::string texturePath)
    {
        // lookup the texture and see if its in the map
        std::map<std::string, GLTexture>::iterator mit = mm_textureMap.find(texturePath);

        //Check if texture is not already in cache
        if (mit == mm_textureMap.end())
        {
            //load the texture
            GLTexture newTexture = TextureLoader::load(texturePath);

            //insert new texture into the map
            mm_textureMap.insert(make_pair(texturePath, newTexture));

            //return the newly loaded texture
            return newTexture;
        }

        return mit->second;
    }
}
