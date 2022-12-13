#include "ButtonComponent.hpp"

#include "InputManager.hpp"
#include "SoundManager.hpp"
#include "ScriptEngine.hpp"
#include "Entity.hpp"
#include "Vector2D.hpp"

#include "SpriteComponent.hpp"

namespace Vigilant {

    ButtonComponent::ButtonComponent(Entity* owner) : Component(owner), LuaListener() {
		// From MenuButton class
		// Starting frame is 1st one
        //m_currentFrame = MOUSE_OUT;

    }

    void ButtonComponent::update(float deltaTime) {
        Vector2D mousePosition = TheInputManager::Instance()->getMouseCoords();
        auto sprite = owner->getComponent<SpriteComponent>();

        if (mousePosition.getX() < (owner->transform->getX() + sprite->getWidth() * owner->transform->getScaleX()) && (mousePosition.getX() > owner->transform->getX())
                && mousePosition.getY() < (owner->transform->getY() + sprite->getHeight() * owner->transform->getScaleY()) && mousePosition.getY() > owner->transform->getY()) {
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
}
