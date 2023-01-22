#include "UILabelComponent.hpp"

#include "Entity.hpp"
#include "Logger.hpp"
#include "Engine.hpp"

namespace Vigilant {

    UILabelComponent::UILabelComponent(Entity* owner) : Component(owner) {
        texture = nullptr;
    }

    UILabelComponent::~UILabelComponent() {
        // Instead of destroying it here, it could be added to texture map and then
        // cleared from it here, which would destroy texture too,
        // unless we resize screen or change text and then we need a new texture,
        // need to make sure we destroy old one as well
        SDL_DestroyTexture(texture);
    }

    void UILabelComponent::setValue(std::string text) {
        value = text;
        // TODO: check here if texture is not already set to delete it
        // We will assume texture is set here for performance, but error will be logged
        // in rendering system in case texture is not set
        texture = TextureManager::Instance()->getTextTexture(fontId, text, textColor);

        //Get image dimensions
        SDL_QueryTexture(texture, NULL, NULL, &ui.width, &ui.height);
        // Or could use TTF_SizeText ? which won't require texture to find width and height

        // Calculate text offset based on allignment, ignores offset if alligned central
        ui.refresh();
    }


    void UILabelComponent::render() {
        TheTextureManager::Instance()->draw(texture, ui.x, ui.y, isAbsolute);
    }

    void UILabelComponent::update(float deltaTime) {
        if (lifetime != -1.0f) {
            if (lifetimeTime.read() > lifetime) {
                owner->destroy();
            }
        }
    }

}
