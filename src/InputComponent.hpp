#ifndef __InputComponent__
#define __InputComponent__

#include "Component.hpp"

namespace Vigilant {

    class InputComponent : public Component {
        public:
            InputComponent(Entity* owner);
            void update(float deltaTime);
    };
}

#endif // __InputComponent__