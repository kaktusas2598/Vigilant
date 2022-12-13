#include "ButtonComponent.hpp"

#include "InputManager.hpp"
#include "SoundManager.hpp"
#include "Entity.hpp"
#include "Vector2D.hpp"

#include "SpriteComponent.hpp"

namespace Vigilant {

    ButtonComponent::ButtonComponent(Entity* owner) : Component(owner) {
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
                callback();
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
