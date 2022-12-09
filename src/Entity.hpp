#ifndef __Entity__
#define __Entity__

#include <vector>
#include <memory>

#include "LoaderParams.hpp"

#include "Component.hpp"
#include "TransformComponent.hpp"

#include "SpriteComponent.hpp"
#include "InputComponent.hpp"
#include "PhysicsComponent.hpp"
#include "ColliderComponent.hpp"
//#include "ScriptComponent.hpp"

#include "Logger.hpp"
#include "LuaScript.hpp"

// Temp for testing
#include <iostream>

namespace Vigilant {
	/** \brief Entity
	*
	*  Different from traditional ECS model where Entity is basically just a number
	*  TODO: Future plans: Implement Entity Component System to decouple things like Renderer, Audio and so on
	*/
	class Entity {

		public:
			// TODO: once Entity isn't pure abstract anymore, move this to public
            Entity() {
				transform = addComponent<TransformComponent>();
				alive = true;
				livingCount++;
			}

			virtual void update(float deltaTime) {
				for(int i = components.size() - 1; i >= 0; i--) {
					components[i]->update(deltaTime);
				}
			} //= 0;
			virtual void draw(float deltaTime) {
				for(int i = components.size() - 1; i >= 0; i--) {
					components[i]->render();
				}
			} //= 0;
            virtual void clean() {} //= 0;

			virtual void load(const LoaderParams *params) {} //= 0;

			void collide(Entity* entity) {
			}

			void destroy() {
				alive = false;
			}

			template <typename T>
			std::shared_ptr<T> addComponent() {
				if (!std::is_base_of<Component, T>::value) {
					Logger::Instance()->error("Must derive from Component class!");
				}

				// Checks if there is already a component of this type
				for (auto& component: components) {
					if (std::dynamic_pointer_cast<T>(component)) {
						return std::dynamic_pointer_cast<T>(component);
					}
				}

				std::shared_ptr<T> newComponent = std::make_shared<T>(this);
				components.push_back(newComponent);

				return newComponent;
			}

			template <typename T>
			std::shared_ptr<T> getComponent() {
				for (auto& component: components) {
					if (std::dynamic_pointer_cast<T>(component)) {
						return std::dynamic_pointer_cast<T>(component);
					}
				}
				return nullptr;
			}

			bool isAlive() { return alive; }

			std::shared_ptr<TransformComponent> transform;
			static int livingCount;

		// protected:
		// Had to uncomment this to bind Entity class to lua, this probably ruins previously used
		// inheritance and polymorphism principle for game objects, but scrolling background and menu buttons still appear to work
			virtual ~Entity(){
				std::cout << "Virtual private Entity destructor called." << std::endl;
				livingCount--;
			}
		private:

			std::vector<std::shared_ptr<Component>> components;
			bool alive;
	};
}

#endif // __Entity__
