#include "Player.hpp"

#include <string>
#include <SDL2/SDL.h>
#include "InputManager.hpp"

namespace Vigilant {
    Player::Player(): SDLEntity() {}

    void Player::load(const LoaderParams* params) {
        SDLEntity::load(params);
    }

    void Player::draw(float deltaTime) {
        SDLEntity::draw(deltaTime);
    }

    void Player::update(float deltaTime) {
        handleInput();

        // m_position.setX(m_position.getX() + 1); //TODO: TEMP
        m_currentFrame = int(((SDL_GetTicks() / 100) % 6));
        SDLEntity::update(deltaTime);
    }

    void Player::handleInput() {
        Vector2D target = TheInputManager::Instance()->getMouseCoords();
        m_velocity = target - m_position;
        m_velocity /= 50;
    }

    void Player::clean() {}
}