#include "ColliderComponent.hpp"

// TODO: move width and height to transform component?
#include "SpriteComponent.hpp"
#include "Entity.hpp"

#include <iostream>

namespace Vigilant {

    ColliderComponent::ColliderComponent(Entity* owner) : Component(owner), LuaListener() { }

    void ColliderComponent::update(float deltaTime) {
        auto sprite = owner->getComponent<SpriteComponent>();
        float colliderOffsetX = 0, colliderOffsetY = 0;
        if (sprite) {
            colliderOffsetX = (sprite->getWidth() - collider.w)/2 * owner->transform->getScaleX();
            colliderOffsetY = (sprite->getHeight() - collider.h)/2 * owner->transform->getScaleY();
            collider.w = sprite->getWidth() * owner->transform->getScaleX();
            collider.h = sprite->getHeight() * owner->transform->getScaleY();
        }
        collider.x = owner->transform->getX() + colliderOffsetX;
        collider.y = owner->transform->getY() + colliderOffsetY;
    }
}
