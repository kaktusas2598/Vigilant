#ifndef __Particle__
#define __Particle__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vector2D.hpp"

#define MIN_LIFE_TO_INTERPOLATE 15

namespace Vigilant {

    class Particle {
        public:
            Particle();

            void init(
                    Vector2D pos,
                    float startSpeed, float endSpeed, float angle,
                    double rotSpeed,
                    float startSize,
                    float endSize,
                    Uint32 life,
                    SDL_Rect textureRect,
                    SDL_Color startColor,
                    SDL_Color endColor,
                    SDL_BlendMode blendMode
                    //bool vortexSensitive
                    );


            void update(float deltaTime);
            void render();

            Particle* getNext() const { return state.next; }
            void setNext(Particle* next) { state.next = next; }

            bool isAlive() { return life > 0; }

            SDL_Color rgbInterpolation(SDL_Color startColor, float timeStep, SDL_Color endColor);
            float interpolateRange(float min, float timeStep, float max);

            void calculateParticlePosition(float deltaTime);

        private:
            //TODO: vortex?

            /*  This is the only variable we care about no matter if
             *  the particle is alive or dead */
            Uint32 life = 0;

            union ParticleInfo {
                /* This struct holds the state of the particle when
                 * it's being update (it's still alive).*/
                struct ParticleState
                {
                    Uint32 startLife;
                    Vector2D pos;
                    Vector2D startVel;
                    Vector2D endVel;
                    Vector2D currentVel;
                    float currentSize, startSize, endSize;
                    float ageRatio;
                    float angle;
                    double startRotSpeed;
                    double currentRotSpeed;
                    SDL_Rect rect;
                    SDL_Rect rectSize;
                    SDL_Color startColor;
                    SDL_Color endColor;
                    SDL_BlendMode blendMode;
                    //bool vortexSensitive;
                    float t;

                    ParticleState() {}
                } live;

                /* If the particle is dead, then the 'next' member comes
                 * into play and the struct it's not used. This pointer
                 * called 'next' holds a pointer to the next available
                 * particle after this one. */
                Particle* next;

                ParticleInfo() {}
            } state;
    };
}

#endif // __Particle__
