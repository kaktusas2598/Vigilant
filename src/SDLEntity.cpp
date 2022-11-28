#include "SDLEntity.hpp"
#include "TextureManager.hpp"

namespace Vigilant {
    SDLEntity::SDLEntity(): Entity(), m_position(0,0), m_velocity(0,0), m_acceleration(0,0) {}

    void SDLEntity::load(const LoaderParams* params) {
        m_position = Vector2D(params->getX(), params->getY());
	    m_velocity = Vector2D(0, 0);
        m_acceleration = Vector2D(0, 0);
        m_width = params->getWidth();
        m_height = params->getHeight();

        m_textureID = params->getTextureID();
        m_currentRow = 1;
        m_currentFrame = 1;
    }

    void SDLEntity::draw(float deltaTime) {
        if(m_velocity.getX() > 0) {
            TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, SDL_FLIP_HORIZONTAL);
        }
        else {
            TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame);
        }
    }

    void SDLEntity::update(float deltaTime) {
        Vector2D gravityForce{0.0f, 5.0f};

        m_velocity += m_acceleration;
        // m_position += m_velocity;
        m_position += m_velocity * deltaTime;
        m_velocity += gravityForce * deltaTime;

        // transform.x = position.getX();
        // transform.y = position.getY();
        // rigidBody.velocityX = velocity.getX();
        // rigidBody.velocityY = velocity.getY();
    }
}