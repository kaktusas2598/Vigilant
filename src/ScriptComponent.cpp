#include "ScriptComponent.hpp"

#include "Entity.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "ColliderComponent.hpp"
#include "InputComponent.hpp"

namespace Vigilant {

    ScriptComponent::ScriptComponent(Entity* owner) : Component(owner) {}

    void ScriptComponent::init(std::string tag, std::string filename) {
        id = tag;
        fileName = filename;
        script = new LuaScript(fileName);
        state = script->getLuaState();

        luaL_newmetatable(state, "EntityMetaTable");

        lua_pushstring(state, "__gc");
        lua_pushcfunction(state, lua_destroyEntity);
        lua_settable(state, -3);

        // Register C++ -> Lua wrappers
        // Must be done before running lua script
        lua_register(state, "playSound", lua_playSound);
        lua_register(state, "playMusic", lua_playMusic);
        lua_register(state, "createEntity", lua_createEntity);
        lua_register(state, "teleportEntity", lua_teleportEntity);
        lua_register(state, "addSprite", lua_addSprite);
    }

    void ScriptComponent::load() {
        script->open();

        // Getting all components for entity
        std::vector<std::string> componentNames = script->getTableKeys(id);
        for (std::string componentName : componentNames) {
            if (componentName == "sprite") {
                std::string filename = script->get<std::string>(id + ".sprite.filename");
                std::string textureId = script->get<std::string>(id + ".sprite.id");
                int width = script->get<int>(id + ".sprite.width");
                int height =script->get<int>(id + ".sprite.height");
                owner->addComponent<SpriteComponent>();
                owner->getComponent<SpriteComponent>()->load(textureId, width, height);
                bool isAnimated = false;

                // Check for optional properties like animation
                // First find sprite table inside player table
                // This is not great at all, because you have to loop through lua state again
                // both LuaScript::getTableKeys() and lua_next are doing same job here
                lua_getglobal(state, id.c_str());
                lua_pushnil(state);
                while (lua_next(state, -2) != 0) {
                    std::string component = lua_tostring(state, -2);
                    if (component == "sprite") {
                        // try to iterate through sprite component
                        lua_pushnil(state);
                        while (lua_next(state, -2) != 0) {
                            if (lua_isstring(state, -2)) {
                                std::string spriteProp = lua_tostring(state, -2);
                                if (spriteProp == "animation") {
                                    isAnimated = true;
                                    // Now finally try to loop through animations
                                    lua_pushnil(state);
                                    while (lua_next(state, -2) != 0) {
                                        // Inside each animation, push individual key on top of the stack to get value
                                        // and pop last element off to keep stack clean
                                        // TODO: implement LuaScript::getTableValue<T> (const char* key);
                                        // OR: write custom methods for engine only
                                        lua_getfield(state, -1, "name");
                                        std::string name = (std::string)lua_tostring(state, -1);
                                        lua_pop(state, 1);

                                        lua_getfield(state, -1, "numFrames");
                                        int numFrames = (int)lua_tonumber(state, -1);
                                        lua_pop(state, 1);

                                        lua_getfield(state, -1, "row");
                                        int row = (int)lua_tonumber(state, -1);
                                        lua_pop(state, 1);

                                        Animation anim = {row, numFrames};
                                        owner->getComponent<SpriteComponent>()->addAnimation(name, anim);

                                        if (name == "default") {
                                            owner->getComponent<SpriteComponent>()->setAnimation(name);
                                        }

                                        // Pop a key off for next iteration
                                        lua_pop(state, 1);
                                    }
                                }
                            }
                            lua_pop(state, 1);
                        }
                    }
                    lua_pop(state, 1);
                }
                owner->getComponent<SpriteComponent>()->setAnimated(isAnimated);
            } else if (componentName == "physics") {
                float mass = script->get<float>(id + ".physics.mass");
                float velocityX = script->get<float>(id + ".physics.velocityX");
                float velocityY = script->get<float>(id + ".physics.velocityY");
                float aceclerationX = script->get<float>(id + ".physics.aceclerationX");
                float accelerationY = script->get<float>(id + ".physics.accelerationY");
                float gravityX = script->get<float>(id + ".physics.gravityX");
                float gravityY = script->get<float>(id + ".physics.gravityY");
                float frictionCoef = script->get<float>(id + ".physics.friction");

                owner->addComponent<PhysicsComponent>();
                owner->getComponent<PhysicsComponent>()->setMass(mass);
                owner->getComponent<PhysicsComponent>()->setFrictionCoefficient(frictionCoef);
                owner->getComponent<PhysicsComponent>()->load(
                    velocityX,
                    velocityY,
                    aceclerationX,
                    accelerationY,
                    gravityX,
                    gravityY
                );
                // TODO: somehow register functions as listeners here
            } else if (componentName == "input") {
                owner->addComponent<InputComponent>();
            } else if (componentName == "collider") {
                std::string tag = script->get<std::string>(id + ".collider.type");
                int width = script->get<int>(id + ".collider.width");
                int height = script->get<int>(id + ".collider.height");
                owner->addComponent<ColliderComponent>();
                owner->getComponent<ColliderComponent>()->load(tag, width, height);
            }
        }

        // Transform will be included for every entity
        owner->transform->setX(script->get<float>(id + ".transform.X"));
        owner->transform->setY(script->get<float>(id + ".transform.Y"));
        owner->transform->setScaleX(script->get<float>(id + ".transform.scaleX"));
        owner->transform->setScaleY(script->get<float>(id + ".transform.scaleY"));
    }

    void ScriptComponent::update(float deltaTime) {}

    void ScriptComponent::onInput(unsigned int keyID) {
        // Find appropriate lua function and call
    }

    void ScriptComponent::onCollide(Entity* entity) {
        // Find appropriate lua function and call

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
}
