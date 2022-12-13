#include "ScriptEngine.hpp"

#include "Entity.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "ColliderComponent.hpp"
#include "InputComponent.hpp"

#include "Logger.hpp"
#include "EntityManager.hpp"

#include <cassert>
#include <iostream>

namespace Vigilant {

	ScriptEngine* ScriptEngine::s_pInstance = nullptr;

    int ScriptEngine::lua_createEntity(lua_State *L) {
        // Important to understand here that memory is being allocated by Lua and
        // we want to pass it to EntityManager so it can update and render all entities
        Entity* entity = (Entity*)lua_newuserdata(L, sizeof(Entity));
        // Placement new operator takes already allocated memory and calls constructor
        new (entity) Entity();

		EntityManager::Instance()->addEntity(entity);

        luaL_getmetatable(L, "EntityMetaTable");
        lua_setmetatable(L, -2);


        return 1;
    }

    // Will be called once lua state is closed
	int ScriptEngine::lua_destroyEntity(lua_State *L) {
		Entity* entity = (Entity*)lua_touserdata(L, -1);
		//std::cout << "Lua __gc for Entity called" << std::endl;
		// Lua takes care of gc and entity gets removed from entity manager using its id
		// to avoid double freeing of same memory, so we don't need to call destructor explicitly
		// Tried maping entities with ids using map in entity manager but could not get that to work
		EntityManager::Instance()->remove(entity->id->get());

		return 0;
	}


    ScriptEngine::ScriptEngine() {
		// By default we will assume there are no listener functions defined in Lua
		collideListenerDefined = false;
		inputListenerDefined = false;
    }

    void ScriptEngine::init(std::string filename) {
        //id = tag;
        fileName = filename;
        script = new LuaScript(fileName);
        state = script->getLuaState();

        // Setup table for Entity class with __index and __gc metamethods
        lua_newtable(state);
        int entityTableIndex = lua_gettop(state);
        // Push entity table twice because lua_setglobal will pop it off the stack, but we will need table to assign methods
        lua_pushvalue(state, entityTableIndex);
        lua_setglobal(state, "Entity");

        // Set methods for Entity table
        lua_pushcfunction(state, lua_createEntity);
        lua_setfield(state, -2 , "create");
        lua_pushcfunction(state, lua_entityId);
        lua_setfield(state, -2 , "id");
        lua_pushcfunction(state, lua_teleportEntity);
        lua_setfield(state, -2 , "move");
        lua_pushcfunction(state, lua_scaleEntity);
        lua_setfield(state, -2 , "scale");

        lua_pushcfunction(state, lua_addSprite);
        lua_setfield(state, -2 , "addSprite");
        lua_pushcfunction(state, lua_setAbsolutePosition);
        lua_setfield(state, -2 , "setAbsolutePosition");

        lua_pushcfunction(state, lua_addAnimation);
        lua_setfield(state, -2 , "addAnimation");
        lua_pushcfunction(state, lua_setAnimation);
        lua_setfield(state, -2 , "setAnimation");

        lua_pushcfunction(state, lua_addCollider);
        lua_setfield(state, -2 , "addCollider");
        lua_pushcfunction(state, lua_setCollideListener);
        lua_setfield(state, -2 , "setCollideListener");

        lua_pushcfunction(state, lua_addInput);
        lua_setfield(state, -2 , "addInput");
        lua_pushcfunction(state, lua_setInputListener);
        lua_setfield(state, -2 , "setInputListener");

        lua_pushcfunction(state, lua_addButton);
        lua_setfield(state, -2 , "addButton");
        lua_pushcfunction(state, lua_setButtonListener);
        lua_setfield(state, -2 , "setButtonListener");

        lua_pushcfunction(state, lua_addPhysics);
        lua_setfield(state, -2 , "addPhysics");
        lua_pushcfunction(state, lua_addProjectile);
        lua_setfield(state, -2 , "addProjectile");
        // TODO: replace this with __newindex?
        lua_pushcfunction(state, lua_getX);
        lua_setfield(state, -2 , "getX");
        lua_pushcfunction(state, lua_getY);
        lua_setfield(state, -2 , "getY");

        luaL_newmetatable(state, "EntityMetaTable");

        // So Lua knows how to automatically garbage collect entities
        lua_pushstring(state, "__gc");
        lua_pushcfunction(state, lua_destroyEntity);
        lua_settable(state, -3);

        // Will let use Entity OOP way in lua
        lua_pushstring(state, "__index");
        lua_pushvalue(state, entityTableIndex);
        lua_settable(state, -3);
        // Also might want to implement __newindex to write properties oop way eg. entity.x = 10
        // Embedding Lua in C++ #13 tutorial

        // Register C++ -> Lua wrappers
        // Must be done before running lua script
        lua_register(state, "playSound", lua_playSound);
        lua_register(state, "playMusic", lua_playMusic);
        lua_register(state, "getMapWidth", lua_getMapWidth);
        lua_register(state, "getMapHeight", lua_getMapHeight);
        lua_register(state, "getScreenWidth", lua_getScreenWidth);
        lua_register(state, "getScreenHeight", lua_getScreenHeight);
        lua_register(state, "changeState", lua_changeState);

        script->open();
    }

    void ScriptEngine::update(float deltaTime) {
        // luaL_dofile(state, fileName.c_str());
    }

    void ScriptEngine::onInput(std::string listener, int thisId, unsigned int keyID) {
        // Find appropriate lua function and call
        if (!listener.empty()){
            lua_getglobal(state, listener.c_str());
            lua_pushnumber(state, thisId);
            lua_pushnumber(state, keyID);

            if (luaOk(state, lua_pcall(state, 2, 0, 0))) {
                std::cout << "onInput() Called sucessfully!!!" << std::endl;
            }
        }
    }

    void ScriptEngine::onCollide(std::string listener, int thisId, int secondId) {
        if (!listener.empty()){
            lua_getglobal(state, listener.c_str());
            lua_pushnumber(state, thisId);
            lua_pushnumber(state, secondId);

            if (luaOk(state, lua_pcall(state, 2, 0, 0))) {
                std::cout << "onCollide() Called sucessfully!!!" << std::endl;
            }
        }
    }
}
