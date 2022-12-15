#ifndef __ScriptEngine__
#define __ScriptEngine__

#include "LuaScript.hpp"

#include "Entity.hpp"
#include "ProjectileComponent.hpp"
#include "ButtonComponent.hpp"

#include "SoundManager.hpp"
#include "TextureManager.hpp"
#include "Engine.hpp"
#include "IGameState.hpp"

#include <string>

namespace Vigilant {

    // TODO:
    // Must support multiple scripts, global script for common functionality, config scripts
    // Might work similar to TextureManager, maybe recall to ScriptManager?
    // addScript(ScriptType)
	enum ScriptType {
		SCRIPT_CONFIG = 0,
		SCRIPT_GLOBALS = 1,
		SCRIPT_ENTITIES = 2
	};

    class ScriptEngine {
        public:
            ScriptEngine();
            ~ScriptEngine() {
            }

            static ScriptEngine* Instance() {
                if(s_pInstance == 0) {
                    s_pInstance = new ScriptEngine();
                    return s_pInstance;
                }
                return s_pInstance;
            }

            // Scripts will probably not run every frame so this will be empty method
            void update(float deltaTime);

            // Setup metatables and bind c++ methods
            void init(std::string filename);
            // Close all scripts
            void close() {
				script->close();
			}

            // Listeners
            void onInput(std::string listener, int thisId, unsigned int keyID);
			void onCollide(std::string listener, int thisId, int secondId);
            // void onCollide(Entity* entity);

			// Lua API is written in C so only static C++ class methods can be wrapped for lua
            // Alternative way would be to use global functions
            //
            //------------------------------------------------------------------------------------
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

			static int lua_getMapWidth(lua_State *L) {
				int mapWidth = TheEngine::Instance()->getMapWidth();
                lua_pushnumber(L, mapWidth);
				return 1;
			}

            static int lua_getMapHeight(lua_State *L) {
				int mapHeight = TheEngine::Instance()->getMapHeight();
                lua_pushnumber(L, mapHeight);
				return 1;
			}

			static int lua_getScreenWidth(lua_State *L) {
				int screenWidth = TheEngine::Instance()->getScreenWidth();
                lua_pushnumber(L, screenWidth);
				return 1;
			}

            static int lua_getScreenHeight(lua_State *L) {
				int screenHeight = TheEngine::Instance()->getScreenHeight();
                lua_pushnumber(L, screenHeight);
				return 1;
			}

			// TODO: this is temporary, it will need to accept state id as param
			static int lua_changeState(lua_State *L) {
				TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
				return 1;
			}



			static int lua_createEntity(lua_State *L);

			static int lua_destroyEntity(lua_State *L);

			static int lua_entityId(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, -1);
                lua_pushnumber(L, entity->id->get());
				return 1;
			}

			static int lua_getX(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, -1);
                lua_pushnumber(L, entity->transform->getX());
				return 1;
			}

			static int lua_getY(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, -1);
                lua_pushnumber(L, entity->transform->getY());
				return 1;
			}

			static int lua_teleportEntity(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, -3);
				float x = (float)lua_tonumber(L, -2);
				float y = (float)lua_tonumber(L, -1);

				entity->transform->setX(x);
				entity->transform->setY(y);
				return 0;
			}

            static int lua_scaleEntity(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, -3);
				float scaleX = (float)lua_tonumber(L, -2);
				float scaleY = (float)lua_tonumber(L, -1);

				entity->transform->setScaleX(scaleX);
				entity->transform->setScaleY(scaleY);
				return 0;
			}

			static int lua_addSprite(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				std::string id = (std::string)lua_tostring(L, 2);
				std::string fileName = (std::string)lua_tostring(L, 3);
				float width = (int)lua_tonumber(L, 4);
				float height = (int)lua_tonumber(L, 5);

				TheTextureManager::Instance()->load(fileName, id);
				entity->addComponent<SpriteComponent>();
                entity->getComponent<SpriteComponent>()->load(id, width, height);

				return 0;
			}

			static int lua_setAbsolutePosition(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				bool absol = (bool)lua_toboolean(L, 2);

                entity->getComponent<SpriteComponent>()->setAbsolute(absol);

				return 0;
			}


			static int lua_addAnimation(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				std::string name = (std::string)lua_tostring(L, 2);
				int row = (int)lua_tonumber(L, 3);
				int numFrames = (int)lua_tonumber(L, 4);

				Animation anim = {row, numFrames};
                entity->getComponent<SpriteComponent>()->addAnimation(name, anim);

				return 0;
			}

			static int lua_setAnimation(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				std::string name = (std::string)lua_tostring(L, 2);

                entity->getComponent<SpriteComponent>()->setAnimation(name);

				return 0;
			}

			static int lua_addInput(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				entity->addComponent<InputComponent>();

				return 0;
			}

			static int lua_setInputListener(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				std::string listener = (std::string)lua_tostring(L, 2);

				entity->getComponent<InputComponent>()->setListener(listener);

				return 0;
			}

			static int lua_addButton(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				entity->addComponent<ButtonComponent>();

				return 0;
			}

			static int lua_setButtonListener(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				std::string listener = (std::string)lua_tostring(L, 2);

				entity->getComponent<ButtonComponent>()->setListener(listener);

				return 0;
			}


			static int lua_addCollider(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				std::string tag = (std::string)lua_tostring(L, 2);
				int width = (int)lua_tonumber(L, 3);
				int height = (int)lua_tonumber(L, 4);

				entity->addComponent<ColliderComponent>();
				entity->getComponent<ColliderComponent>()->load(tag, width, height);

				return 0;
			}

			static int lua_setCollideListener(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				std::string listener = (std::string)lua_tostring(L, 2);

				entity->getComponent<ColliderComponent>()->setListener(listener);

				return 0;
			}

			static int lua_addPhysics(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				float mass = (int)lua_tonumber(L, 2);
				float frictionCoef = (int)lua_tonumber(L, 3);

				entity->addComponent<PhysicsComponent>();
				entity->getComponent<PhysicsComponent>()->setMass(mass);
				entity->getComponent<PhysicsComponent>()->setFrictionCoefficient(frictionCoef);
				// entity->getComponent<PhysicsComponent>()->load(float velX, float velY, float accX, float accY, float gX, float gY)
				return 0;
			}

			static int lua_addProjectile(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				Entity* shooter = (Entity*)lua_touserdata(L, 2);
				int speed = (int)lua_tonumber(L, 3);
				int range = (int)lua_tonumber(L, 4);
				Vector2D velocity = shooter->getComponent<PhysicsComponent>()->getVelocity();
				velocity.normalize();
				velocity *= speed;
				entity->addComponent<ProjectileComponent>()->load(shooter, range, speed, velocity);
				return 0;
			}

            // Temporary place, this method should go to dedicated scripting classes
			bool luaOk(lua_State* L, int call) {
                if (call != LUA_OK) {
                    std::string errorMessage = lua_tostring(L, -1);
					std::cout << errorMessage << std::endl;
                    Logger::Instance()->error(errorMessage.c_str());
                    return false;
                }
				return true;
            }
            //------------------------------------------------------------------------------------

        private:
        	static ScriptEngine* s_pInstance;

            lua_State* state;
            // This will not work for multiple scripts
            LuaScript* script;
            std::string fileName;
            // This identifies which table to get from lua ascript, dont like this for some reason
            std::string id;

            bool collideListenerDefined;
            bool inputListenerDefined;
    };
}

#endif // __ScriptEngine__