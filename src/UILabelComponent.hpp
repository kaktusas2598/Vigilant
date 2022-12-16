#ifndef __UILabel__
#define __UILabel__

#include "Component.hpp"

#include "TextureManager.hpp"

namespace Vigilant {

	enum Align {
		ALIGN_LEFT = 0,
		ALIGN_CENTER = 1,
		ALIGN_RIGHT = 2
	};

	enum VerticalAlign {
		ALIGN_UP = 0,
		ALIGN_MIDDLE = 1,
		ALIGN_DOWN = 2
	};

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
