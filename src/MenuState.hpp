#ifndef __MenuState__
#define __MenuState__

#include <vector>

#include "IGameState.hpp"
#include "IEntity.hpp"

namespace Vigilant {

    class MenuState: public IGameState {
        public:
            int getNextStateIndex() const { return 1; }
            int getPrevStateIndex() const { return 1; }
            void build() {}
            void destroy() {}
            void onEntry() {}
            void onExit() {}
            void update(float deltaTime) {}
            void draw(float deltaTime) { }		
        protected:

            typedef void(*Callback)();
            virtual void setCallbacks(const std::vector<Callback>& callbacks) = 0;
            
            std::vector<Callback> m_callbacks;
    };
}

#endif // __MenuState__