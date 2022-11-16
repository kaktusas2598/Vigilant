#ifndef __ObjecLayer__
#define __ObjecLayer__

#include "Layer.hpp"
#include <vector>

namespace Vigilant {

    class IEntity;

    class ObjectLayer : public Layer {
        public:
            virtual void update();
            virtual void render();

            std::vector<IEntity*>* getEntities() { return &entities; };
        private:
            std::vector<IEntity*> entities;
    };
}

#endif // __ObjecLayer__