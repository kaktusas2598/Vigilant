#ifndef __MainMenuState__
#define __MainMenuState__

#include "IGameState.hpp"
#include "Entity.hpp"

namespace Vigilant {

    class MainMenuState: public IGameState {
        public:
            int getNextStateIndex() const { return 1; }
            int getPrevStateIndex() const { return 0; }

            void build() {}
            void destroy() {}
            void onEntry();
            void onExit();

            void update(float deltaTime);
            void draw(float deltaTime);
        private:

            static const std::string menuID;
            std::vector<Entity*> gameEntities;
    };
}

#endif // __MainMenuState__
