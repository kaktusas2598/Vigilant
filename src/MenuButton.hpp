#ifndef __MenuButton__
#define __MenuButton__

#include <string>

#include "SDLEntity.hpp"
#include "EntityFactory.hpp"

namespace Vigilant {
	class MenuButton : public SDLEntity {
		public:
			MenuButton();

			void draw(float deltaTime);
			void update(float deltaTime);
			void clean();
			void load(const LoaderParams *params);

            void setCallback(void(*fn)()) { callback = fn; }
            int getCallbackID() { return callbackID; }
        private:
            enum buttonState {
                MOUSE_OUT = 0,
                MOUSE_OVER = 1,
                CLICKED = 2
            };

            // Function pointer used for button callback action
            void(*callback)();
            bool isReleased;
            int callbackID;

	};

    class MenuButtonCreator: public BaseCreator {
        IEntity* createEntity() const {
            return new MenuButton();
        }
    };
}

#endif // __MenuButton__