#ifndef __BehaviourComponent__
#define __BehaviourComponent__

#include "Component.hpp"
#include "Entity.hpp"
#include "Manipulators.hpp"

#include <vector>
#include <memory>

namespace Vigilant {

    class BehaviourComponent : public Component {
        public:
            BehaviourComponent(Entity* owner);

            void update(float deltaTime);

            void move(Vector2D target, float time) {
                newManipulators.emplace_back(std::make_shared<MoveManipulator>(owner, target, time));
            }

        private:
            std::vector<std::shared_ptr<Manipulator>> manipulators;
            // This is protect in case manipulators are added while looping
            std::vector<std::shared_ptr<Manipulator>> newManipulators;
    };
}

#endif // __BehaviourComponent__
