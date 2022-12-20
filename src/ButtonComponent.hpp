#ifndef __ButtonComponent__
#define __ButtonComponent__

#include "Component.hpp"
#include "UI.hpp"
#include "TextureManager.hpp"

namespace Vigilant {

    /**
     * This Component is problematic because it shares some stuff like texture with Sprite
     * and alignment options as UILabel
     */
    class ButtonComponent : public Component {
        public:
            ButtonComponent(Entity* owner);

            void load(std::string id, int w, int h, int x = 0, int y = 0) {
                ui.x = x;
                ui.y = y;
                ui.width = w;
                ui.height = h;
                ui.offsetX = x;
                ui.offsetY = y;
                textureID = id;
            }

            void render();
            void update(float deltaTime);

            UI ui;
        private:

            std::string textureID;

            enum buttonState {
                MOUSE_OUT = 0,
                MOUSE_OVER = 1,
                CLICKED = 2
            };

            bool isReleased;
    };
}

#endif // __ButtonComponent__
