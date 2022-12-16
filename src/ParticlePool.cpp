#include "ParticlePool.hpp"

#include <cassert>

namespace Vigilant {

	// This pool constructor sets our particles to available
	ParticlePool::ParticlePool(Emitter* emitter) {
		// Fill the pool according to poolSize needed for the emitter
		poolSize = emitter->getPoolSize();
		particleArray = new Particle[poolSize];

		// The first particle is available
		firstAvailable = &particleArray[0];

		// Each particle points to the next one
		for (int i = 0; i < poolSize - 1; i++)
			particleArray[i].setNext(&particleArray[i + 1]);

		// The last particle points to nullptr indicating the end of the vector
		particleArray[poolSize - 1].setNext(nullptr);
	}

	ParticlePool::~ParticlePool() {
		delete[] particleArray;
		particleArray = nullptr;
	}

	void ParticlePool::generate(
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
	) {
		// Check if the pool is not full
		if (firstAvailable == nullptr) {
			return;
		}
		//assert(firstAvailable != nullptr);

		// Remove it from the available list
		Particle* newParticle = firstAvailable;
		firstAvailable = newParticle->getNext();

		// Initialize new alive particle
		newParticle->init(pos, startSpeed, endSpeed, angle, rotSpeed, startSize, endSize, life, textureRect, startColor, endColor, blendMode);
	}

	bool ParticlePool::update(float deltaTime) {
		bool ret = false;

		for (int i = 0; i < poolSize; i++) {
			if (particleArray[i].isAlive()) {
				particleArray[i].update(deltaTime);
				particleArray[i].render();
				ret = true;
			// if a particle dies it becomes the first available in the pool
			} else {
				// Add this particle to the front of the vector
				particleArray[i].setNext(firstAvailable);
				firstAvailable = &particleArray[i];
			}
		}

		return ret;
	}
}
