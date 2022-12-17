#ifndef __Entity__
#define __Entity__

#include <vector>
#include <memory>

#include "Component.hpp"
#include "TransformComponent.hpp"
#include "IdComponent.hpp"

#include "Logger.hpp"
#include "LoaderParams.hpp"

namespace Vigilant {
	/** \brief Entity
	*
	*  Different from traditional ECS model where Entity is basically just a number
	*  TODO: Future plans:
	*	* Implement systems and system signatures
	*   * Implement render groups
	*   * Clenup older code
	*/
	class Entity {

		public:
            Entity() {
				// Init component array for safety
				for (auto& c: componentArray) { c = nullptr; }
				id = addComponent<IdComponent>();
				transform = addComponent<TransformComponent>();
			}

			virtual void update(float deltaTime) {
				for (auto& c: components) {
					c->preUpdate(deltaTime);
					c->update(deltaTime);
				}
			}
			virtual void draw(float deltaTime) {
				for (auto& c: components) {
					c->render();
				}
			}
			// Deprecated
            virtual void clean() {}
			// Deprecated
			virtual void load(const LoaderParams *params) {}

			void destroy() {
				alive = false;
			}

			template <typename T> bool hasComponent() const {
				return componentBitset[getComponentTypeID<T>()];
			}

			//template <typename T, typename... TArgs>
			//T& addComponent(TArgs&&... mArgs) {
				//T* c(new T(std::forward<TArgs>(mArgs)...));
				//c->owner = this // need to rename owner to entity
				//	std::shared_ptr<Component> newComponent = std::make_shared<T>(this);
				//components.emplace_back(std::move(newComponent));

				//componentArray[getComponentTypeID<T>()] = c;
				//componentBitset[getComponentTypeID<T>()] = true;
				//
				//c->init();
				//
				//return *c;
			//}

			template <typename T>
			T* addComponent() {
			// std::shared_ptr<T> addComponent() {
				if (!std::is_base_of<Component, T>::value) {
					Logger::Instance()->error("Must derive from Component class!");
				}

				if (hasComponent<T>()) {
					return getComponent<T>();
				}
				// Checks if there is already a component of this type
				//for (auto& component: components) {
					//if (std::dynamic_pointer_cast<T>(component)) {
						//return std::dynamic_pointer_cast<T>(component);
					//}
				//}

				std::shared_ptr<T> newComponent = std::make_shared<T>(this);
				components.push_back(newComponent);

				// TODO: do I need to use unique_ptr here? Because shared_ptr will create a copy
				// Seems to actually work at the moment
				// So do I even need std::vector for components at this point
				componentArray[getComponentTypeID<T>()] = newComponent.get();
				componentBitset[getComponentTypeID<T>()] = true;

				newComponent->init();

				return newComponent.get();
			}

			//template <typename T> T& getComponent() {
				//auto ptr(componentArray[getComponentTypeID<T>()]);
				//return *static_cast<T*>(ptr);
			//}

			template <typename T> T* getComponent() {
				auto ptr(componentArray[getComponentTypeID<T>()]);
				if (ptr != nullptr)
					return static_cast<T*>(ptr);
				else
					return nullptr;
			}

			// template <typename T>
			// std::shared_ptr<T> getComponent() {
			// 	for (auto& component: components) {
			// 		if (std::dynamic_pointer_cast<T>(component)) {
			// 			return std::dynamic_pointer_cast<T>(component);
			// 		}
			// 	}
			// 	return nullptr;
			// }

			bool isAlive() { return alive; }

			// std::shared_ptr<TransformComponent> transform;
			// std::shared_ptr<IdComponent> id;
			TransformComponent* transform;
			IdComponent* id;

			~Entity(){}
		private:
			std::vector<std::shared_ptr<Component>> components;
			bool alive = true;

			ComponentArray componentArray;
			ComponentBitSet componentBitset;
	};
}

#endif // __Entity__
