#ifndef __InputComponent__
#define __InputComponent__

#include "Component.hpp"
#include <string>

namespace Vigilant {

    class InputComponent : public Component {
        public:
            InputComponent(Entity* owner);
            void update(float deltaTime);

            void setListener(std::string listener) { listenerName = listener; }
            std::string getListener() { return listenerName; }
        private:
            // Name of Lua function which will act as onInput listener
			std::string listenerName;
    };
}

#endif // __InputComponent__
