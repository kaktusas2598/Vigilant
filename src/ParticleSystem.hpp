#ifndef __ParticleSystem__
#define __ParticleSystem__

#include "tinyxml.h"
#include <string>
#include <list>
#include "Emitter.hpp"
#include "Vector2D.hpp"
#include <SDL2/SDL.h>

namespace Vigilant {

	class Emitter;

	struct EmitterData {
		Vector2D angleRange = { 0.0f, 0.0f };
		float startSpeed = 0.0f, endSpeed = 0.0f;
		float startSize = 0.0f, endSize = 0.0f;
		uint32_t emitNumber = 0u;
		uint32_t emitVariance = 0u;
		uint32_t maxParticleLife = 0u;
		SDL_Rect textureRect = { 0, 0 };
		double lifetime = -1.0f;
		SDL_Color startColor = { 0, 0, 0, 0 };
		SDL_Color endColor = { 0, 0, 0, 0 };
		SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
		double rotSpeed = 0;

		//Randoms
		Vector2D rotSpeedRand = { 0.0f, 0.0f };
		Vector2D startSpeedRand = { 0.0f, 0.0f };
		Vector2D endSpeedRand = { 0.0f, 0.0f };
		Vector2D emitVarianceRand = { 0.0f, 0.0f };
		Vector2D lifeRand = { 0.0f, 0.0f };
		Vector2D startSizeRand = { 0.0f, 0.0f };
		Vector2D endSizeRand = { 0.0f, 0.0f };

		// Vortex
		//bool vortexSensitive = false;
	};

	// Completely independent from ECS particle system
	class ParticleSystem {
		public:
			Emitter* addEmitter(Vector2D position, std::string type);
			void destroyEmitter(Emitter* emitter);
			// Destroy all emmiters
			void destroy();

			void clean();

			void update(float deltaTime);
			void render(float deltaTime);

			static ParticleSystem* Instance() {
            if(s_pInstance == 0) {
                s_pInstance = new ParticleSystem();
                return s_pInstance;
            }
            return s_pInstance;
        }

		private:
			static ParticleSystem* s_pInstance;

			std::list<Emitter*> emittersList;
			std::list<Emitter*> emittersToDestroy;
	};
}

#endif
