#include "BehaviourComponent.hpp"

#include <algorithm>
#include "ScriptEngine.hpp"

namespace Vigilant {

    BehaviourComponent::BehaviourComponent(Entity* owner) : Component(owner) {}

    void BehaviourComponent::update(float deltaTime) {
        // Update manipulators
        for (auto& m: newManipulators) {
            manipulators.push_back(std::move(m));
        }
        newManipulators.clear();

        for (auto& m: manipulators) {
            if (m->update(deltaTime)) {
                // Manipulation complete, resoume coroutine
                ScriptEngine::Instance()->issueNextTask(owner->id->get());
            }
        }

        // Remove completed manipulations
        manipulators.erase(
                std::remove_if(manipulators.begin(), manipulators.end(),
                    [](const std::shared_ptr<Manipulator>& m)
                    { return m->complete; } ), manipulators.end()
                );
        //std::erase_if(manipulators, [](const std::shared_ptr<Manipulator>& m)
        //{ return m->complete; } );
    }

}
