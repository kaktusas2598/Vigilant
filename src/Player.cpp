#include "Player.hpp"

#include <string>
#include <SDL2/SDL.h>
#include "InputManager.hpp"
#include "SoundManager.hpp"

namespace Vigilant {
    Player::Player(): SDLEntity() {}

    void Player::load(const LoaderParams* params) {
        SDLEntity::load(params);
    }

    void Player::draw(float deltaTime) {
        SDLEntity::draw(deltaTime);
    }

    void Player::update(float deltaTime) {
        handleInput(deltaTime);

        // m_position.setX(m_position.getX() + 1); //TODO: TEMP
        m_currentFrame = int(((SDL_GetTicks() / 100) % 6));
        SDLEntity::update(deltaTime);
    }

    void Player::handleInput(float deltaTime) {
        // Vector2D target = TheInputManager::Instance()->getMouseCoords();
        // m_velocity = target - m_position;
        // m_velocity /= 100;

        if (TheInputManager::Instance()->isKeyDown(SDLK_w)) {
            m_position.setY( m_position.getY() - (10.0f * deltaTime));
        }
        if (TheInputManager::Instance()->isKeyDown(SDLK_a)) {
            m_position.setX( m_position.getX() - (10.0f * deltaTime));
            // TODO
            // sprite.flipped = true;
        }
        if (TheInputManager::Instance()->isKeyDown(SDLK_s)) {
            m_position.setY( m_position.getY() + (10.0f * deltaTime));
        }
        if (TheInputManager::Instance()->isKeyDown(SDLK_d)) {
            m_position.setX( m_position.getX() + (10.0f * deltaTime));
            // sprite.flipped = false;
        }
        if (TheInputManager::Instance()->isKeyPressed(SDLK_SPACE)) {
            // TODO: should be triggered by event or lua script
            TheSoundManager::Instance()->playSound("jump", 0);
            m_velocity.setY(m_velocity.getY() -20.0f);
            // transform.x +=  (10.0f * deltaTime);
        }
    }

    void Player::clean() {}
}