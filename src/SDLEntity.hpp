#ifndef __SDLEntity__
#define __SDLEntity__

#include "Entity.hpp"
#include "Vector2D.hpp"

namespace Vigilant {
    class SDLEntity : public Entity
    {
        public:
            SDLEntity();
            virtual void draw(float deltaTime);
            virtual void update(float deltaTime);
            virtual void clean() {};
            virtual void load(const LoaderParams *params);

            Vector2D& getPosition() { return m_position; }
            int getWidth() { return m_width; }
            int getHeight() { return m_height; }
        protected:
            Vector2D m_position;
            Vector2D m_velocity;
            Vector2D m_acceleration;

            int m_width;
            int m_height;

            int m_currentRow;
            int m_currentFrame;
            int m_numFrames;

            std::string m_textureID;
    };
}

#endif //  __SDLEntity__