#ifndef __UI__
#define __UI__

#include "Engine.hpp"

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

    // Manages UI component position based on alignment
    struct UI {
        int width, height;
        // Calculated offsets
        int x, y;
        // Original Offsets
        int offsetX, offsetY;

        Align horizontal = ALIGN_LEFT;
        VerticalAlign vertical = ALIGN_UP;

        void setAlignment(Align h = ALIGN_LEFT, VerticalAlign v = ALIGN_UP) {
            horizontal = h;
            vertical = v;
            refresh();
        }

        void refresh() {
            // Calculate offsets based on allignment, must be called on window resize event
            switch (horizontal) {
                case Align::ALIGN_CENTER:
                    x = (Engine::Instance()->getScreenWidth() - width)/2 + offsetX;
                    break;
                case Align::ALIGN_RIGHT:
                    x = Engine::Instance()->getScreenWidth() - width - offsetX;
                    break;
                case Align::ALIGN_LEFT:
                    x = offsetX;
                default:
                    break;
            }
            switch (vertical) {
                case VerticalAlign::ALIGN_MIDDLE:
                    y = (Engine::Instance()->getScreenHeight() - height)/2 + offsetY;
                    break;
                case VerticalAlign::ALIGN_UP:
                    y = offsetY;
                    break;
                case VerticalAlign::ALIGN_DOWN:
                    y = Engine::Instance()->getScreenHeight() - height - offsetY;
                default:
                    break;
            }
        }
    };

}

#endif // __UI__
