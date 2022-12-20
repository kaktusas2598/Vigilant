#ifndef __UILabel__
#define __UILabel__

#include "Component.hpp"
#include "UI.hpp"

#include "TextureManager.hpp"

namespace Vigilant {

    class UILabelComponent: public Component {
        public:
            UILabelComponent(Entity* owner);

            // TODO: Define float lifeTime and update method, so that we can destroy label after some time
            // like displaying damage given for example
            // X and Y will be used as offset depending on allignment, except for central allignments
            void load(int x, int y, std::string text, std::string font, SDL_Color& color) {
                ui.x = x;
                ui.y = y;
                ui.offsetX = x;
                ui.offsetY = y;
                fontId = font;
                textColor = color;
                setValue(text);
            }

            void render();

            // Called everytime text is updated to generate Texture and calculate new width and height
            void setValue(std::string text);

            UI ui;
        private:
            std::string fontId;
            std::string value;
            SDL_Color textColor;
            SDL_Texture* texture;
    };
}

#endif // __UILabel__
