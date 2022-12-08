#ifndef __ScriptComponent__
#define __ScriptComponent__

#include "Component.hpp"
#include "LuaScript.hpp"
// #include "EntityManager.hpp"

#include "Entity.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"

namespace Vigilant {

    class ScriptComponent : public Component {
        public:
            ScriptComponent(Entity* owner);
            ~ScriptComponent() {
                if (state) {
                    lua_close(state);
                }
            }
            // Scripts will probably not run every frame so this will be empty method
            void update(float deltaTime);

            // Setup metatables and bind c++ methods
            void init(std::string tag, std::string filename);
            // Load entity components from table and register listeners
            void load();

            // Listeners
            void onInput(unsigned int keyID);
            void onCollide(Entity* entity);

            /***
             * Load Script?
             * Call specific function? (listener??)
             * Register listeners????
             * Load entity using tables??
             * 
             * 
             * 
            */

           
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

			// All of these lua binders below are work in progress
			// while I learn how to properly use metatables and bind engine to lua
			static int lua_createEntity(lua_State *L) {
				Entity* entity = (Entity*)lua_newuserdata(L, sizeof(Entity));
				// Placement new operator takes already allocated memory and calls constructor
				new (entity) Entity();
                // Somehow need to use this as well
				// EntityManager::Instance()->addEntity(entity);

				luaL_getmetatable(L, "EntityMetaTable");
				lua_setmetatable(L, -2);

				return 1;
			}

            // Will be called once lua state is closed
			static int lua_destroyEntity(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, -1);
				entity->~Entity();
				return 0;
			}

			static int lua_teleportEntity(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, -3);
				float x = (float)lua_tonumber(L, -2);
				float y = (float)lua_tonumber(L, -1);

				entity->transform->setX(x);
				entity->transform->setY(y);
				return 0;
			}

			static int lua_addSprite(lua_State *L) {
				Entity* entity = (Entity*)lua_touserdata(L, 1);
				std::string id = (std::string)lua_tostring(L, 2);
				std::string fileName = (std::string)lua_tostring(L, 3);
				float width = (int)lua_tonumber(L, 4);
				float height = (int)lua_tonumber(L, 5);

				TheTextureManager::Instance()->load(fileName, id);
				entity->addComponent<SpriteComponent>()->load(id, width, height);

				return 0;
			}

			static int lua_addAnimation(lua_State *L) {
				// 4 params:
				// Entity - user data
				// name - string
				// row - number
				// frames - number
				return 0;
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
            //------------------------------------------------------------------------------------

        private:
            lua_State* state;
            LuaScript* script;
            std::string fileName;
            // This identifies which table to get from lua ascript, dont like this for some reason
            std::string id;
    };
}

#endif // __ScriptComponent__