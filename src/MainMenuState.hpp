#ifndef __MainMenuState__
#define __MainMenuState__

#include "IEntity.hpp"
#include "MenuState.hpp"

namespace Vigilant {

    class MainMenuState: public MenuState {
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
            virtual void setCallbacks(const std::vector<Callback>& callbacks);
            // call back functions for menu items
            static void s_menuToPlay();
            static void s_exitFromMenu();

            static const std::string menuID;
            std::vector<IEntity*> gameEntities;
    };
}

#endif // __MainMenuState__