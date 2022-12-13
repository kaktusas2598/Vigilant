#ifndef __ParticlePool__
#define __ParticlePool__

#include "Particle.hpp"
#include "Emitter.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

	class Emitter;

	class ParticlePool {
		public:
			ParticlePool(Emitter* emitter);
			virtual ~ParticlePool();

			// Generates a new particle each time it's called
			void generate(
					Vector2D pos,
					float startSpeed,
					float endSpeed,
					float angle,
					float rotSpeed,
					float startSize,
					float endSize,
					uint32_t life,
					SDL_Rect textureRect,
					SDL_Color startColor,
					SDL_Color endColor,
					SDL_BlendMode blendMode
					//bool vortexSensitive
				);


			// Update (move and draw) particles in the pool. If there are no particles alive returns false
			bool update(float deltaTime);

			void render();
		private:

			int poolSize = 0;
			Particle* firstAvailable;
			Particle* particleArray = nullptr;
	};
}

#endif // __ParticlePool__
