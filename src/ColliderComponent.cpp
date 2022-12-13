#include "ColliderComponent.hpp"

// TODO: move width and height to transform component?
#include "SpriteComponent.hpp"
#include "Entity.hpp"

namespace Vigilant {

    ColliderComponent::ColliderComponent(Entity* owner) : Component(owner), LuaListener() { }

    void ColliderComponent::update(float deltaTime) {
        collider.x = owner->transform->getX();
        collider.y = owner->transform->getY();
    }
}
