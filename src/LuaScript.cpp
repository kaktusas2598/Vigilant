#include "LuaScript.hpp"
#include "ErrorHandler.hpp"

namespace Vigilant {

    LuaScript::LuaScript(const std::string& fileName) {
        L = luaL_newstate();
        if (luaL_loadfile(L, fileName.c_str()) || lua_pcall(L, 0, 0, 0)) {
            exitWithError("Could not load script " + fileName);
            L = 0; 
        }

        if (L)
            luaL_openlibs(L);
    }

    LuaScript::~LuaScript() {
        if (L) {
            lua_close(L);
        }
    }

    void LuaScript::printError(const std::string& variableName, const std::string& reason) {
        //TODO: handle this better
        exitWithError(reason);
    }

    // TODO: add methods for string and float arrays
    std::vector<int> LuaScript::getIntVector(const std::string& name) {
        std::vector<int> v;
        // First check if variable is defined in global table
        lua_getglobal(L, name.c_str());
        if (lua_isnil(L, -1)) {
            return std::vector<int>();
        }

        lua_pushnil(L);
        while (lua_next(L, -2)) {
            v.push_back((int) lua_tonumber(L, -1));
            lua_pop(L, 1);
        }
        clean();
        return v;
    }

}