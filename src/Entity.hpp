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

#include "Logger.hpp"
#include "LuaScript.hpp"

// Temp for testing
#include "SoundManager.hpp"
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

			void destroy() {
				alive = false;
			}



			// Temporary place, this method should go to dedicated scripting classes
			bool luaOk(lua_State* L, int call) {
                if (call != LUA_OK) {
                    std::string errorMessage = lua_tostring(L, -1);
                    Logger::Instance()->error(errorMessage.c_str());
                    return false;
                }
				return true;
            }

			// Testing lua wrapper for SoundManager.playSound()
			// Lua API is written in C so only static C++ class methods can be wrapped for lua
			// Alternative way would be to use global functions
			static int lua_playSound(lua_State *L) {
				std::string soundID = (std::string)lua_tostring(L, 1);
				TheSoundManager::Instance()->playSound(soundID, 0);
				return 0;
			}

			static int lua_playMusic(lua_State *L) {
				std::string soundID = (std::string)lua_tostring(L, 1);
				TheSoundManager::Instance()->playMusic(soundID, 0);
				return 0;
			}


			static int lua_createEntity(lua_State *L) {
				Entity* entity = (Entity*)lua_newuserdata(L, sizeof(Entity));
				entity->transform->setX(0);
				entity->transform->setY(0);
				entity->transform->setScaleX(0);
				entity->transform->setScaleY(0);
				return 1;
			}

			static int lua_addSprite(lua_State *L) {
				std::string id = (std::string)lua_tostring(L, 1);
				std::string fileName = (std::string)lua_tostring(L, 2);

				Entity* entity = (Entity*)lua_newuserdata(L, sizeof(Entity));
				entity->addComponent<SpriteComponent>();
				// entity->getComponent<SpriteComponent>()->
				return 1;
			}

			static int lua_addAnimation(lua_State *L) {
				// 4 params:
				// Entity - user data
				// name - string
				// row - number
				// frames - number
				return 0;
			}

			// --------------------------------

			void loadScript(std::string id, std::string fileName) {
				LuaScript script(fileName);

				// Register C++ -> Lua wrappers
				// Must be done before running lua script
				lua_register(script.getLuaState(), "playSound", lua_playSound);
				lua_register(script.getLuaState(), "playMusic", lua_playMusic);
				// lua_register(script.getLuaState(), "createEntity", lua_createEntity);

				script.open();

				// Getting all components for entity
				std::vector<std::string> componentNames = script.getTableKeys(id);
				for (std::string componentName : componentNames) {
					if (componentName == "sprite") {
						std::string filename = script.get<std::string>(id + ".sprite.filename");
						std::string textureId = script.get<std::string>(id + ".sprite.id");
						int width = script.get<int>(id + ".sprite.width");
						int height =script.get<int>(id + ".sprite.height");
						addComponent<SpriteComponent>();
						getComponent<SpriteComponent>()->load(textureId, width, height);
						bool isAnimated = false;

						// Check for optional properties like animation
						// First find sprite table inside player table
						lua_getglobal(script.getLuaState(), id.c_str());
						lua_pushnil(script.getLuaState());
						while (lua_next(script.getLuaState(), -2) != 0) {
							std::string component = lua_tostring(script.getLuaState(), -2);
							if (component == "sprite") {
								// try to iterate through sprite component
								lua_pushnil(script.getLuaState());
								while (lua_next(script.getLuaState(), -2) != 0) {
									if (lua_isstring(script.getLuaState(), -2)) {
										std::string spriteProp = lua_tostring(script.getLuaState(), -2);
										if (spriteProp == "animation") {
											isAnimated = true;
											// Now finally try to loop through animations
											lua_pushnil(script.getLuaState());
											while (lua_next(script.getLuaState(), -2) != 0) {
												// Inside each animation, push individual key on top of the stack to get value
												// and pop last element off to keep stack clean
												// TODO: implement LuaScript::getTableValue<T> (const char* key);
												// OR: write custom methods for engine only
												lua_getfield(script.getLuaState(), -1, "name");
												std::string name = (std::string)lua_tostring(script.getLuaState(), -1);
												lua_pop(script.getLuaState(), 1);

												lua_getfield(script.getLuaState(), -1, "numFrames");
												int numFrames = (int)lua_tonumber(script.getLuaState(), -1);
												lua_pop(script.getLuaState(), 1);

												lua_getfield(script.getLuaState(), -1, "row");
												int row = (int)lua_tonumber(script.getLuaState(), -1);
												lua_pop(script.getLuaState(), 1);

												Animation anim = {row, numFrames};
												getComponent<SpriteComponent>()->addAnimation(name, anim);

												if (name == "default") {
													getComponent<SpriteComponent>()->setAnimation(name);
												}

												// Pop a key off for next iteration
												lua_pop(script.getLuaState(), 1);
											}
										}
									}
									lua_pop(script.getLuaState(), 1);
								}
							}
							lua_pop(script.getLuaState(), 1);
						}
						getComponent<SpriteComponent>()->setAnimated(isAnimated);
					} else if (componentName == "physics") {
						float mass = script.get<float>(id + ".physics.mass");
						float velocityX = script.get<float>(id + ".physics.velocityX");
						float velocityY = script.get<float>(id + ".physics.velocityY");
						float aceclerationX = script.get<float>(id + ".physics.aceclerationX");
						float accelerationY = script.get<float>(id + ".physics.accelerationY");
						float gravityX = script.get<float>(id + ".physics.gravityX");
						float gravityY = script.get<float>(id + ".physics.gravityY");
						float frictionCoef = script.get<float>(id + ".physics.friction");

						addComponent<PhysicsComponent>();
						getComponent<PhysicsComponent>()->setMass(mass);
						getComponent<PhysicsComponent>()->setFrictionCoefficient(frictionCoef);
						getComponent<PhysicsComponent>()->load(
							velocityX,
							velocityY,
							aceclerationX,
							accelerationY,
							gravityX,
							gravityY
						);
					} else if (componentName == "input") {
						addComponent<InputComponent>();
					} else if (componentName == "collider") {
						std::string tag = script.get<std::string>(id + ".collider.type");
						int width = script.get<int>(id + ".collider.width");
						int height = script.get<int>(id + ".collider.height");
						addComponent<ColliderComponent>();
						getComponent<ColliderComponent>()->load(tag, width, height);
					}
				}

				// Transform will be included for every entity
				transform->setX(script.get<float>(id + ".transform.X"));
				transform->setY(script.get<float>(id + ".transform.Y"));
				transform->setScaleX(script.get<float>(id + ".transform.scaleX"));
				transform->setScaleY(script.get<float>(id + ".transform.scaleY"));


				// lua_istable(script.getLuaState(), -1)


				/*if(luaOk(L, luaL_dofile(L, fileName.c_str()))) {

					// Test function example
					lua_getglobal(L, "testFunc");
					if (lua_isfunction(L, -1)) {
						lua_pushnumber(L, 3.33f);
						lua_pushnumber(L, 6.66f);

						if (luaOk(L, lua_pcall(L, 2, 1, 0))) {
							funcResult = (float)lua_tonumber(L, -1);
						}
					}
				}*/

				// lua_close(script.getLuaState());

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

		protected:
			virtual ~Entity(){
				std::cout << "Virtual private Entity destructor called." << std::endl;
				livingCount--;
			}

			std::vector<std::shared_ptr<Component>> components;
			bool alive;
	};
}

#endif // __Entity__
