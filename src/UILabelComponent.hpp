#ifndef __UILabel__
#define __UILabel__

#include "Component.hpp"
#include "Utils.hpp"

#include "TextureManager.hpp"

namespace Vigilant {

    class UILabelComponent: public Component {
        public:
            UILabelComponent(Entity* owner);

            // X and Y will be used as offset depending on allignment, except for central allignments
            void load(int x, int y, std::string text, std::string font, SDL_Color& color) {
                position.x = x;
                position.y = y;
                offsetX = x;
                offsetY = y;
                fontId = font;
                textColor = color;
                setValue(text);
            }

            // To update on screen resize, this is bad practise because all UI components need same method
            void reload() {
                setValue(value);
            }

            void render();

            // Called everytime text is updated to generate Texture and calculate new width and height
            void setValue(std::string text);

            void setAlignment(Align h = ALIGN_LEFT, VerticalAlign v = ALIGN_UP) {
                horizontal = h;
                vertical = v;
                setValue(value);
            }
        private:
            SDL_Rect position;
            int offsetX, offsetY;
            std::string fontId;
            std::string value;
            SDL_Color textColor;
            SDL_Texture* texture;

            Align horizontal = ALIGN_LEFT;
            VerticalAlign vertical = ALIGN_UP;
    };
}

#endif // __UILabel__
