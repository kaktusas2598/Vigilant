#include "UILabelComponent.hpp"

#include "Entity.hpp"
#include "Logger.hpp"
#include "Engine.hpp"

namespace Vigilant {

    UILabelComponent::UILabelComponent(Entity* owner) : Component(owner) {}

    void UILabelComponent::setValue(std::string text) {
        value = text;
        //Render text surface
        // TODO: read about TTF_RenderText_Blended() as well
        SDL_Surface* textSurface = TTF_RenderText_Solid(TheTextureManager::Instance()->getFont(fontId) , text.c_str(), textColor);
        if( textSurface == NULL ) {
            Logger::Instance()->error(TTF_GetError());
        }
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface( TheTextureManager::Instance()->getRenderer(), textSurface );
        if( texture == NULL ) {
            Logger::Instance()->error(SDL_GetError());
        }
        else {
            //Get image dimensions
            SDL_QueryTexture(texture, NULL, NULL, &ui.width, &ui.height);
            // Or could use TTF_SizeText ? which won't require texture to find width and height
        }

        // Get rid of old surface
        SDL_FreeSurface( textSurface );

        // Calculate text offset based on allignment, ignores offset if alligned central
        ui.refresh();
    }


    void UILabelComponent::render() {
        // TODO: all SDL_RenderCopy and SDL_RenderCopyEx calls, including the ones in SpriteComponent should go to
        // future rendering system class
        SDL_Rect dest;
        dest.x = ui.x;
        dest.y = ui.y;
        dest.w = ui.width;
        dest.h = ui.height;
        SDL_RenderCopy(TheTextureManager::Instance()->getRenderer(), texture, NULL, &dest);
    }

}
