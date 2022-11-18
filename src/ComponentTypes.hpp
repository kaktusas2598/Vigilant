#ifndef __ComponentTypes__
#define __ComponentTypes__

#include "Vector2D.hpp"
#include <string>

namespace Vigilant {

    /*
     * Describing different component types in an entity compoent system
     */

    //FIXME: for some reason can't initialise structs with Vector2D
    struct Transform {
        float x;
        float y;
        // Vector2D position;
        // TODO: scale, rotation
        
    };
    struct RigidBody {
        float velocityX;
        float velocityY;
        float accelerationX;
        float accelerationY;
        // Vector2D velocity;
        // Vector2D acceleration;
    };

    struct Sprite {
        int width;
        int height;
        std::string textureID;
        int numFrames = 0;
    };

    struct Gravity {
        float forceX;
        float forceY;
        // Vector2D force;
    };
}

#endif // __ComponentTypes__