#include "Emitter.hpp"

#include <time.h>
#include "Logger.hpp"

namespace Vigilant {

	Emitter::Emitter(Vector2D pos, EmitterData data) {
		srand(time(NULL));

		// Particles size and movement
		this->angleRange = data.angleRange;
		this->startSpeed = data.startSpeed;
		this->endSpeed = data.endSpeed;
		this->startSize = data.startSize;
		this->endSize = data.endSize;
		this->pos = pos;
		this->rotSpeed = data.rotSpeed;

		// Particle emission calculations
		this->emitNumber = data.emitNumber;
		this->emitVariance = data.emitVariance;
		this->maxParticleLife = data.maxParticleLife;
		maxParticlesPerFrame = data.emitNumber + data.emitVariance;

		// Pool size calculations
		poolSize = maxParticlesPerFrame * (maxParticleLife + 1);
		emitterPool = new ParticlePool(this);

		// Color and render properties
		this->textureRect = data.textureRect;
		this->startColor = data.startColor;
		this->endColor = data.endColor;
		this->blendMode = data.blendMode;
		timeStep = 1.0f / (float)maxParticleLife;

		// Emission properties
		active = true;
		lifetime = data.lifetime;

		// Random control parameters
		rotSpeedRand = data.rotSpeedRand;
		startSpeedRand = data.startSpeedRand;
		endSpeedRand = data.endSpeedRand;
		emitVarianceRand = data.emitVarianceRand;
		lifeRand = data.lifeRand;
		startSizeRand = data.startSizeRand;
		endSizeRand = data.endSizeRand;

		// Vortex control parameters
		//vortexSensitive = data.vortexSensitive;

		if (this->lifetime != -1.0f && this->lifetime > 0.0f)
			lifeTimer.start();
	}

	Emitter::~Emitter()
	{
		delete emitterPool;
		emitterPool = nullptr;
	}


	void Emitter::update(float deltaTime) {
		if (active) {

			// Particle generation from pool
			// TODO: load rand tanges from xml
			emissionRate = (int)(emitNumber + emitVariance ); //* rangeRandomNum(emitVarianceRand.getX(), emitVarianceRand.getY()));

			for (int i = 1; i <= emissionRate; i++) {
				float tmpStartSpeed = startSpeed * rangeRandomNum(startSpeedRand.getX(), startSpeedRand.getY());
				float tmpEndSpeed = endSpeed * rangeRandomNum(endSpeedRand.getX(), endSpeedRand.getY());
				float randAngle = rangeRandomNum(angleRange.getX(), angleRange.getY());
				float randStart = startSize * rangeRandomNum(startSizeRand.getX(), startSizeRand.getY());
				float randEnd = startSize * rangeRandomNum(startSizeRand.getX(), startSizeRand.getY());
				float randRadius = rangeRandomNum(randStart, randEnd);
				double randRotSpeed = rotSpeed * rangeRandomNum(rotSpeedRand.getX(), rotSpeedRand.getY());

				emitterPool->generate(pos, tmpStartSpeed, tmpEndSpeed, randAngle, randRotSpeed, randRadius, endSize, maxParticleLife, textureRect, startColor, endColor, blendMode);
				timeStep += timeStep;
			}
		}

		// Emission timing calculations
		if (stopTime > 0.0f && !active) {
			emissionTime = 0.0f;
			if (stopTimer.read() >= stopTime) {
				active = true;
				stopTime = 0.0f;
			}
		}

		if (emissionTime > 0.0f) {
			stopTime = 0.0f;
			if (emissionTimer.read() >= emissionTime) {
				active = false;
				emissionTime = 0.0f;
			}
		}

		if (lifetime > 0.0f) {
			if (lifeTimer.read() >= lifetime) {
				active = false; // and remove emitter
				lifetime = 0.0f;
			}
		}

	}

	void Emitter::render(float deltaTime) {
		// Check if all particles are dead and remove emitter
		if (!emitterPool->update(deltaTime) && lifetime == 0.0f) {
			Logger::Instance()->info("Desotrying emitter");
			ParticleSystem::Instance()->destroyEmitter(this);
		}
	}

}

