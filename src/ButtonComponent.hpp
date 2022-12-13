#ifndef __ButtonComponent__
#define __ButtonComponent__

#include "Component.hpp"

namespace Vigilant {

    class ButtonComponent : public Component {
        public:
            ButtonComponent(Entity* owner);

            void load(int cId, void(*fn)()) {
            	callbackID = cId;
            	callback = fn;
            }

            void render() {}
            void update(float deltaTime);

            void setCallback(void(*fn)()) { callback = fn; }
            int getCallbackID() { return callbackID; }
        private:
            enum buttonState {
                MOUSE_OUT = 0,
                MOUSE_OVER = 1,
                CLICKED = 2
            };

            // Function pointer used for button callback action
            void(*callback)();
            bool isReleased;
            int callbackID;
    };
}

#endif // __ButtonComponent__
