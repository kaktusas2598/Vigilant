#ifndef __BehaviourComponent__
#define __BehaviourComponent__

#include "Component.hpp"
#include "Manipulators.hpp"

#include <vector>
#include <memory>
#include <algorithm>

namespace Vigilant {

    class BehaviourComponent : public Component {
        public:
            BehaviourComponent(Entity* owner) : Component(owner) {}

            void update(float deltaTime) {
                // Update manipulators
                for (auto& m: newManipulators) {
                    manipulators.push_back(std::move(m));
                }
                newManipulators.clear();

                for (auto& m: manipulators) {
                    if (m->update(deltaTime)) {
                        // TODO: call ScriptEngine here?
                        // manipulation completed, request next one
                        //lua_getglobal(L, "issueNextTask");
                        //if (lua_isfunction(L, -1)) {
                            //lua_pushlightuserdata(L, this);
                            //lua_pushlightuserdata(L, m->entity);
                            //if(!luaOk(lua_pcall(L, 2, 1, 0))) {
                                //// behaviour function error or script error
                            //}
                        //}
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
