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

	void ColliderComponent::load(std::string tag, int width, int height) {
		type = tag;
		collider.w = width;
		collider.h = height;
		collider.x = 0;
		collider.y = 0;

		// Collider MUST be loaded AFTER the sprite!
		// Seems to save some FPS, but logic would brake if scale is changed after this or sprite changes
		auto sprite = owner->getComponent<SpriteComponent>();
		if (sprite) {
			xOffset = (sprite->getWidth() - collider.w)/2 * owner->transform->getScaleX();
			yOffset = (sprite->getHeight() - collider.h)/2 * owner->transform->getScaleY();
		}
		collider.w = width * owner->transform->getScaleX();
		collider.h = height * owner->transform->getScaleY();
	}


	void ColliderComponent::update(float deltaTime) {
		collider.x = owner->transform->getX() + xOffset;
		collider.y = owner->transform->getY() + yOffset;
	}

}
