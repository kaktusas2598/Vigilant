#ifndef __IdComponent__
#define __IdComponent__

#include "Component.hpp"

namespace Vigilant {

    class IdComponent : public Component {
        public:
            IdComponent(Entity* owner);
            ~IdComponent();

            int get() const { return id; }

            std::string getTag() const { return tag; }
            void setTag(std::string t) { tag = t; }
            std::string getType() const { return type; }
            void setType(std::string t) { type = t; }

        private:
            static int count;
            int id;
            std::string tag; ///< Optional tag to identify specific entitites
            std::string type;
    };
}

#endif // __IdComponent__
