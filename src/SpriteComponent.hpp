#ifndef __SpriteComponent__
#define __SpriteComponent__

#include "Component.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <map>

#include "Logger.hpp"

namespace Vigilant {

    enum AnimationState {
		STILL = 0,
		ONE_SHOT,
		LOOP
	};

    struct Animation {
        int row; // Row in spritesheet for concrete animation
        int numFrames; // Number of Frames
        bool loop = false;
        // TODO:
        // int column; // Column in tilesheet where animation starts
        // bool loop; // Will denote if animation plays once or loops
    };

    class SpriteComponent : public Component {
        public:
            SpriteComponent(Entity* owner);

            void load(std::string id, int w, int h) {
                textureID = id;
                width = w;
                height = h;
            }

            void render();
            void update(float deltaTime);
            void draw(SDL_RendererFlip flip);
            void drawFrame(SDL_RendererFlip flip);

            Animation getAnimation(std::string name) {
                return animationList[name];
            }
            void addAnimation(std::string name, Animation animation) {
                animationList.emplace(name, animation);
            }
            // If one shot is true, animaion onlu plays once
            void playAnimation(std::string name, bool oneShot = false) {
                isAnimated = true;
                if (oneShot)
                	animationState = ONE_SHOT;
				else
					animationState = LOOP;
                if (animationList.find(name) != animationList.end())
                    currentAnimation = name;
                else
                    Logger::Instance()->error("Animation not found.");
            }
            void stopAnimation(std::string) {
                isAnimated = false;
                animationState = STILL;
            }

            AnimationState getAnimationState() const { return animationState; }

            void setWidth(int w) { width = w; }
            void setHeight(int h) { height = h; }
            void setTextureID(const std::string& id) { textureID = id; }
            void setAnimated(bool animate) { isAnimated = animate; }
            void setFlipped(bool flip) { isFlipped = flip; }

            void setAbsolute(bool absol) { isAbsolute = absol; }

            int getWidth() const { return width; }
            int getHeight() const { return height; }
        private:
            bool isAnimated;
            AnimationState animationState;
            std::string currentAnimation;
            bool isFlipped;
            int currentFrame;

            int width;
            int height;
            std::string textureID;
            std::map<std::string, Animation> animationList;

            // Is sprite always rendered in the same place relative to the screen
            bool isAbsolute;
    };
}

#endif // __SpriteComponent__
