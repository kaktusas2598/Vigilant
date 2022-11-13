#include "Enemy.hpp"
#include "InputManager.hpp"

namespace Vigilant {
    Enemy::Enemy(): SDLEntity() {}

    void Enemy::load(const LoaderParams* params) {
        SDLEntity::load(params);
    }

    void Enemy::draw(float deltaTime) {
        SDLEntity::draw(deltaTime);
    }

    void Enemy::update(float deltaTime) {
        m_position.setX(m_position.getX() + 1); //TODO: TEMP
        m_currentFrame = int(((SDL_GetTicks() / 100) % 6));
        SDLEntity::update(deltaTime);
    }

    void Enemy::clean() {}
}