#ifndef __LuaListener__
#define __LuaListener__

#include <string>

namespace Vigilant {

    /**
     * Some Components will also be Listeners extending from this class so they can register lua listener function name
     *
     * ScriptEngine will dispatch events from various places in Engine and use listener name from here
     */
    class LuaListener {
        public:
            LuaListener(){}

            void setListener(std::string listenerFunctionName) { listener = listenerFunctionName; }
            std::string getListener() { return listener; }
        protected:
            // Name of Lua function which will act as a listener
            std::string listener;
    };
}

#endif // __LuaListener__
