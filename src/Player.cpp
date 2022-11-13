#include <string>
#include <SDL2/SDL.h>

#include "Player.hpp"

namespace Vigilant {
    Player::Player(): SDLEntity() {}

    void Player::load(const LoaderParams* params) {
        SDLEntity::load(params);
    }

    void Player::draw(float deltaTime) {
        SDLEntity::draw(deltaTime);
    }

    void Player::update(float deltaTime) {
        m_position.setX(m_position.getX() + 1);
        m_currentFrame = int(((SDL_GetTicks() / 100) % 6));
    }

    void Player::clean() {}
}