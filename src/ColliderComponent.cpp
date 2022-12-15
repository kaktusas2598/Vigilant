#include "ColliderComponent.hpp"

// TODO: move width and height to transform component?
#include "SpriteComponent.hpp"
#include "Entity.hpp"

#include <iostream>

namespace Vigilant {

	ColliderComponent::ColliderComponent(Entity* owner) : Component(owner), LuaListener() {
		xOffset = 0;
		yOffset = 0;
	}

	void ColliderComponent::update(float deltaTime) {
		// Only calculate collider box offset if it wasn't calculated before to save time
		// but collider must be initialised after sprite!
		if (xOffset == 0 || yOffset == 0) {
			auto sprite = owner->getComponent<SpriteComponent>();
			if (sprite) {
				xOffset = (sprite->getWidth() - collider.w)/2 * owner->transform->getScaleX();
				yOffset = (sprite->getHeight() - collider.h)/2 * owner->transform->getScaleY();
				collider.w = sprite->getWidth() * owner->transform->getScaleX();
				collider.h = sprite->getHeight() * owner->transform->getScaleY();
			}
		}
		collider.x = owner->transform->getX() + xOffset;
		collider.y = owner->transform->getY() + yOffset;
	}

}
