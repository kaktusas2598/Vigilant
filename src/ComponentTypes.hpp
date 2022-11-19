#ifndef __ComponentTypes__
#define __ComponentTypes__

#include "Vector2D.hpp"
#include <string>
#include <unordered_map>

namespace Vigilant {

    /*
     * Describing different component types in an entity compoent system
     */

    //FIXME: for some reason can't initialise structs with Vector2D
    struct Transform {
        float x;
        float y;
        float scaleX = 1.0f;
        float scaleY = 1.0f;
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

    /*
     Animation in spritesheet propertries

     1. animations might be in one or multiple files
     2. each frame is a sprite
     4. flip must be indicated for some animations like walk left/right
     5. is animation on the loop or one shot?
    
    */
    struct Animation {
        std::string filename;
        int numFrames;
        int numRow;
        bool flipped = false;
    };

    struct Sprite {
        int width;
        int height;
        std::string textureID;
        // std::string fileName;
        int numFrames = 0; // Num of frames in a row
        bool flipped = false;
        std::string animation = "";
        // std::map<std::string id, int row> animations; flip param is missing
        // TODO:
        bool isAnimated = true;
        int currentFrame = 0;
        int currentRow = 1; // Row determines type of animation usually

        // Different animations
        // sprite h/w vs spritesheet hw
        // animation IDs
    };

    struct Gravity {
        float forceX;
        float forceY;
        // Vector2D force;
    };

    // TODO: rename back to Player once Player class is gone
    struct Playable {
        int health = 100;
        bool isJumping = false;
    };
}

#endif // __ComponentTypes__