#include "ColliderComponent.hpp"

// TODO: move width and height to transform component?
#include "SpriteComponent.hpp"
#include "Entity.hpp"

#include "TextureManager.hpp"

#include <iostream>

namespace Vigilant {

    ColliderComponent::ColliderComponent(Entity* owner) : Component(owner) {
        xOffset = 0;
        yOffset = 0;
    }

    void ColliderComponent::load(int width, int height, int x, int y) {
        collider.w = width;
        collider.h = height;
        collider.x = 0;
        collider.y = 0;

        // Collider MUST be loaded AFTER the sprite!
        auto sprite = owner->getComponent<SpriteComponent>();

        // If no offset provided assume collider is centered within sprite frame
        if (x == 0)
            xOffset = (sprite->getWidth() - collider.w)/2 * owner->transform->getScaleX();
        else
            xOffset = x * owner->transform->getScaleX();
        if (y == 0)
            yOffset = (sprite->getHeight() - collider.h)/2 * owner->transform->getScaleY();
        else
            yOffset = y * owner->transform->getScaleY();

        collider.w = width * owner->transform->getScaleX();
        collider.h = height * owner->transform->getScaleY();
    }


    void ColliderComponent::update(float deltaTime) {
        collider.x = owner->transform->getX() + xOffset;
        collider.y = owner->transform->getY() + yOffset;
    }

    void ColliderComponent::render() {
        if (DebugConsole::colliderVisibility) {
            // Will render red, half transparent collider box, outline
            TextureManager::Instance()->drawRect(collider, {255, 0, 0, 125}, false);
        }
    }
}
