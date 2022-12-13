#ifndef __ButtonComponent__
#define __ButtonComponent__

#include "Component.hpp"
#include "LuaListener.hpp"

namespace Vigilant {

    class ButtonComponent : public Component, public LuaListener {
        public:
            ButtonComponent(Entity* owner);

            void load() {}

            void render() {}
            void update(float deltaTime);

        private:
            enum buttonState {
                MOUSE_OUT = 0,
                MOUSE_OVER = 1,
                CLICKED = 2
            };

            bool isReleased;
    };
}

#endif // __ButtonComponent__
