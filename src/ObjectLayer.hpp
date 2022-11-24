#ifndef __ObjecLayer__
#define __ObjecLayer__

#include "Layer.hpp"
#include <vector>

namespace Vigilant {

    class Entity;

    class ObjectLayer : public Layer {
        public:
            virtual void update();
            virtual void render();

            std::vector<Entity*>* getEntities() { return &entities; };
        private:
            std::vector<Entity*> entities;
    };
}

#endif // __ObjecLayer__