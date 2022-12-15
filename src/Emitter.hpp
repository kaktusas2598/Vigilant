#ifndef __Emitter__
#define __Emitter__

#include "Vector2D.hpp"
#include "Timer.hpp"
#include "ParticleSystem.hpp"
#include "ParticlePool.hpp"
#include <SDL2/SDL.h>

namespace Vigilant {

	class ParticlePool;
	struct EmitterData;

	class Emitter {
		public:
			Emitter(Vector2D pos, EmitterData data);
			virtual ~Emitter();

			void update(float deltaTime);
			void render(float deltaTime);

			// Generates random number between given range
			float rangeRandomNum(float min = -1.0f, float max = 1.0f) {
				float random = ((float)rand()) / (float)RAND_MAX;
				float diff = max - min;
				float r = random * diff;

				return min + r;
			}

			int getPoolSize() const { return poolSize; }

			// Starts emission specified by timer, if not emission time is infinite
			void startEmission(double timer = -1.0f) {
				if (!active) {
					active = true;
					emissionTime = timer;
					//emissionTimer.Start();
				}
			}

			// Stops emission specified by timer, if not emission stop time is infinite
			void stopEmission(double timer = 0.0f) {
				if (active)
				{
					active = false;
					stopTime = timer;
					//stopTimer.Start();
				}
			}

			// Emitter move methods
			void MoveEmitter(Vector2D newPos) { pos = newPos; }
			Vector2D GetEmitterPos() const { return pos; }

		private:
			// Particles size and movement
			Vector2D pos = { 0.0f, 0.0f };
			Vector2D angleRange = { 0.0f, 360.0f };
			float startSpeed = 0.0f;
			float endSpeed = 0.0f;
			float startSize = 0.0f;
			float endSize = 0.0f;
			double rotSpeed = 0;

			// Particles emission
			int emissionRate = 0;
			Uint32 emitNumber = 0u;
			Uint32 emitVariance = 0u;
			Uint32 maxParticleLife = 0u;
			Uint32 maxParticlesPerFrame = 0u;

			// Pool
			ParticlePool* emitterPool = nullptr;
			int poolSize = 0;

			// Emission time
			Timer stopTimer;
			Timer emissionTimer;
			Timer lifeTimer;
			double stopTime = 0.0f;
			double emissionTime = 0.0f;
			double lifetime = -1.0f;
			bool active = false;

			// Color and render properties
			SDL_Color startColor = { 0, 0, 0, 0 };
			SDL_Color endColor = { 0, 0, 0, 0 };
			SDL_Rect textureRect;
			SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
			float timeStep = 0.0f;

			// Random control parameters
			Vector2D rotSpeedRand = { 0.0f, 0.0f };
			Vector2D startSpeedRand = { 0.0f, 0.0f };
			Vector2D endSpeedRand = { 0.0f, 0.0f };
			Vector2D emitVarianceRand = { 0.0f, 0.0f };
			Vector2D lifeRand = { 0.0f, 0.0f };
			Vector2D startSizeRand = { 0.0f, 0.0f };
			Vector2D endSizeRand = { 0.0f, 0.0f };

			// Vortex control parameters
			//bool vortexSensitive = false;
	};
}
#endif // __Emitter__
