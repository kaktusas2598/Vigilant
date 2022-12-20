#include "ButtonComponent.hpp"

#include "InputManager.hpp"
#include "SoundManager.hpp"
#include "ScriptEngine.hpp"
#include "Entity.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

    ButtonComponent::ButtonComponent(Entity* owner) : Component(owner), LuaListener() {
        // Todo, need basic animation here, Animation struct could go to a new file
        // From MenuButton class
        // Starting frame is 1st one
        //m_currentFrame = MOUSE_OUT;
    }

    void ButtonComponent::update(float deltaTime) {
        Vector2D mousePosition = TheInputManager::Instance()->getMouseCoords();

        // TODO: should use transform position instead?
        if (mousePosition.getX() < (ui.x + ui.width * owner->transform->getScaleX()) && (mousePosition.getX() > ui.x)
                && mousePosition.getY() < (ui.y + ui.height * owner->transform->getScaleY()) && mousePosition.getY() > ui.y) {
            if (TheInputManager::Instance()->isKeyPressed(SDL_BUTTON_LEFT)/* && isReleased*/) {
                //m_currentFrame = CLICKED;
                TheSoundManager::Instance()->playSound("button", 0);
                //callback();
                // TODO: third argument is SDL Key Code, do something about this, overload onInput or something
                ScriptEngine::Instance()->onInput(getListener(), owner->id->get(), 0);
                isReleased = false;
            } /*else if (TheInputManager::Instance()->isKeyDown(SDL_BUTTON_LEFT)) {
                isReleased = true;
                m_currentFrame = MOUSE_OVER;
                }*/
        } else {
            //m_currentFrame = MOUSE_OUT;
        }

    }

    void ButtonComponent::render() {
        SDL_Rect srcRect;
        SDL_Rect destRect;

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = ui.width;
        srcRect.h = ui.height;
        destRect.w = ui.width * owner->transform->getScaleX();
        destRect.h = ui.height * owner->transform->getScaleY();
        destRect.x = ui.x;
        destRect.y = ui.y;

        SDL_RenderCopyEx(
                TheEngine::Instance()->getSDLRenderer(),
                TheTextureManager::Instance()->getTexture(textureID),
                &srcRect,
                &destRect,
                0,
                0,
                SDL_FLIP_NONE
                );
    }
}
