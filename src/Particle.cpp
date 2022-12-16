#include "Particle.hpp"

#define PI  3.14159265359f
#define DEG_2_RAD(a) a * PI / 180.0f
#define RAD_2_DEG(a) a * 180.0f / PI;

#include "Logger.hpp"
#include "TextureManager.hpp"
#include <iostream>

namespace Vigilant {

	Particle::Particle() {}

	void Particle::init(
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
	) {
		state.live.pos = pos;
		state.live.startVel.setX(startSpeed * cos(DEG_2_RAD(angle)));
		state.live.startVel.setY(-startSpeed * sin(DEG_2_RAD(angle)));
		state.live.endVel.setX(endSpeed * cos(DEG_2_RAD(angle)));
		state.live.endVel.setY(-endSpeed * sin(DEG_2_RAD(angle)));
		state.live.startRotSpeed = rotSpeed;
		state.live.currentRotSpeed = rotSpeed;

		// Life properties
		this->life = state.live.startLife = life;
		state.live.currentSize = state.live.startSize = startSize;
		state.live.endSize = endSize;
		state.live.t = 0.0f;

		// Color properties
		state.live.startColor = startColor;
		state.live.endColor = endColor;
		state.live.blendMode = blendMode;
		state.live.rect = state.live.rectSize = textureRect;

		// Vortex
		//state.live.vortexSensitive = vortexSensitive;
		// Add vortex to the system (optional and only one is allowed)
		//if (state.live.vortexSensitive)
			//AddVortex({ 250.0f, 200.0f }, 10.0f, 30.0f);

	}

	void Particle::update(float deltaTime) {
		// Vortex control
		//if (!state.live.vortexSensitive && vortex.scale != 0 && vortex.speed != 0)
			//AddVortex({ 0.0f, 0.0f }, 0.0f, 0.0f);

		// Age ratio is used to interpolate between particle properties
		state.live.ageRatio = (float)life / (float)state.live.startLife;

		// Particle size interpolation
		state.live.currentSize = interpolateRange(state.live.startSize, state.live.t, state.live.endSize);

		// Particle speed interpolation
		state.live.currentVel.setX(interpolateRange(state.live.startVel.getX(), state.live.t, state.live.endVel.getX()));
		state.live.currentVel.setY(interpolateRange(state.live.startVel.getY(), state.live.t, state.live.endVel.getY()));

		// Assign new size to particle rect
		state.live.rectSize.w = state.live.rectSize.h = state.live.currentSize;

		calculateParticlePosition(deltaTime);

		// Decrementing particle life
		life--;
	}

	void Particle::render() {

		// Calculations to determine the current center of particle texture
		SDL_Rect tmpRect = { (int)state.live.startSize, (int)state.live.startSize };
		float centerX = state.live.pos.getX() + ((tmpRect.w - state.live.rectSize.w) / 2.0f);
		float centerY = state.live.pos.getY() + ((tmpRect.h - state.live.rectSize.h) / 2.0f);

		// Color interpolation, only if the particle has enough life
		SDL_Color resColor;

		if (state.live.startLife > MIN_LIFE_TO_INTERPOLATE)
			resColor = rgbInterpolation(state.live.startColor, state.live.t, state.live.endColor);

		// Refactor these into rendering, also particleState needs texture id
		SDL_Texture* texture = TheTextureManager::Instance()->getTexture("particle");
		if (SDL_SetTextureColorMod(texture, resColor.r, resColor.g, resColor.b) != 0)
			Logger::Instance()->error(SDL_GetError());
		if (SDL_SetTextureAlphaMod(texture, resColor.a) != 0)
			Logger::Instance()->error(SDL_GetError());
		if (SDL_SetTextureBlendMode(texture, state.live.blendMode) != 0)
			Logger::Instance()->error(SDL_GetError());

		//TheTextureManager::draw(std::string id, int x, int y, int width, int height, double angle);
		TheTextureManager::Instance()->draw("particle", (int)centerX, (int)centerY, state.live.rectSize.w, state.live.rectSize.h, state.live.currentRotSpeed);
		// Blitting particle on screen
		//BlitParticle(App->psystem->GetParticleAtlas(), (int)centerX, (int)centerY, &state.live.pRect, &state.live.rectSize, resColor, state.live.blendMode, 1.0f, state.live.currentRotSpeed);
		//BlitParticle(SDL_Texture* texture, int x, int y, const SDL_Rect* section, const SDL_Rect* rectSize, SDL_Color color, SDL_BlendMode blendMode, float speed, double angle)
//{
	//SDL_Rect rect;
	//rect.x = (int)(camera.x * speed) + x * scale;
	//rect.y = (int)(camera.y * speed) + y * scale;

	//if (rectSize != NULL)
	//{
		//rect.w = rectSize->w;
		//rect.h = rectSize->h;
	//}
	//else if(section != NULL)
	//{
		//rect.w = section->w;
		//rect.h = section->h;
	//}
	//else
		//SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

	//int px = rect.w / 2;
	//int py = rect.h / 2;

	//rect.w *= scale;
	//rect.h *= scale;

// Color mode setting here

	//if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, NULL, SDL_FLIP_NONE) != 0)
	//{
		//LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		//ret = false;
	//}

	//return ret;
//}

		// Calculating new rotation according to rotation speed
		state.live.currentRotSpeed += state.live.startRotSpeed;

		// Time step increment to interpolate colors
		state.live.t += (1.0f / (float)state.live.startLife);

		if (state.live.t >= 1.0f)
			state.live.t = 0.0f;
	}

	SDL_Color Particle::rgbInterpolation(SDL_Color startColor, float timeStep, SDL_Color endColor) {
		SDL_Color finalColor;

		finalColor.r = startColor.r + (endColor.r - startColor.r) * timeStep;
		finalColor.g = startColor.g + (endColor.g - startColor.g) * timeStep;
		finalColor.b = startColor.b + (endColor.b - startColor.b) * timeStep;
		finalColor.a = startColor.a + (endColor.a - startColor.a) * timeStep;

		return finalColor;
	}

	float Particle::interpolateRange(float min, float timeStep, float max) {
		return min + (max - min) * timeStep;
	}

	void Particle::calculateParticlePosition(float deltaTime) {
		//float dx = state.live.pos.x - vortex.pos.x;
		//float dy = state.live.pos.y - vortex.pos.y;
		//float vx = -dy * vortex.speed;
		//float vy = dx * vortex.speed;
		//float factor = 1.0f / (1.0f + (dx * dx + dy * dy) / vortex.scale);

		//state.live.pos.x += (vx - state.live.currentVel.x) * factor + state.live.currentVel.x * deltaTime;
		//state.live.pos.y += (vy - state.live.currentVel.y) * factor + state.live.currentVel.y * deltaTime;

		state.live.pos.setX(state.live.pos.getX() + state.live.currentVel.getX() + state.live.currentVel.getX() * deltaTime);
		state.live.pos.setY(state.live.pos.getY() + state.live.currentVel.getY() + state.live.currentVel.getY() * deltaTime);
	}

}
