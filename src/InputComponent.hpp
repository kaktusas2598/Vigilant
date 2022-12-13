#ifndef __InputComponent__
#define __InputComponent__

#include "Component.hpp"
#include "LuaListener.hpp"
#include <string>

namespace Vigilant {

    class InputComponent : public Component, public LuaListener {
        public:
            InputComponent(Entity* owner);
            void update(float deltaTime);
    };
}

#endif // __InputComponent__
