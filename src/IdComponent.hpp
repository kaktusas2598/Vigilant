#ifndef __IdComponent__
#define __IdComponent__

#include "Component.hpp"

namespace Vigilant {

    class IdComponent : public Component {
        public:
            IdComponent(Entity* owner);
            ~IdComponent();

            int get() const { return id; }
        private:
            static int count;
            int id;
    };
}

#endif // __IdComponent__