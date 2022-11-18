#include "MenuButton.hpp"
#include "Vector2D.hpp"
#include "InputManager.hpp"
#include "SoundManager.hpp"

namespace Vigilant {
    MenuButton::MenuButton(): SDLEntity() {
        // Starting frame is 1st one
        m_currentFrame = MOUSE_OUT;
    }

    void MenuButton::draw(float deltaTime) {
        SDLEntity::draw(deltaTime);
    }

    void MenuButton::update(float deltaTime) {
        Vector2D mousePosition = TheInputManager::Instance()->getMouseCoords();

        if (mousePosition.getX() < (m_position.getX() + m_width) && (mousePosition.getX() > m_position.getX())
                && mousePosition.getY() < (m_position.getY() + m_height) && mousePosition.getY() > m_position.getY()) {
            if (TheInputManager::Instance()->isKeyPressed(SDL_BUTTON_LEFT)/* && isReleased*/) {
                m_currentFrame = CLICKED;
                TheSoundManager::Instance()->playSound("button", 1);
                callback();
                isReleased = false;
            } /*else if (TheInputManager::Instance()->isKeyDown(SDL_BUTTON_LEFT)) {
                isReleased = true;
                m_currentFrame = MOUSE_OVER;
            }*/
        } else {
            m_currentFrame = MOUSE_OUT;
        }
    }

    void MenuButton::clean() {
        SDLEntity::clean();
    }

    void MenuButton::load(const LoaderParams* params) {
        SDLEntity::load(params);
        callbackID = params->getCallbackID();
        m_currentFrame = MOUSE_OUT;
    }
}