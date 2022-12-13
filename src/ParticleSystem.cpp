#include "ParticleSystem.hpp"

#include "ErrorHandler.hpp"
#include "Logger.hpp"

#include <iostream>

namespace Vigilant {

    ParticleSystem* ParticleSystem::s_pInstance = nullptr;

	Emitter* ParticleSystem::addEmitter(Vector2D position, std::string type) {
		TiXmlDocument xmlDoc;

		Logger::Instance()->info("Loading emitter data");
        if (!xmlDoc.LoadFile("emitters.xml")) {
            exitWithError("Could not load XML state file");
        }

        TiXmlElement *root = xmlDoc.RootElement();
        EmitterData data;

        // Loop through emitter types
        for (TiXmlElement *e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
			std::string emitterType = e->Attribute("type");
			if (type == emitterType) {
				// Correct emitter type found, loop through properties
				for (TiXmlElement *property = e->FirstChildElement(); property != NULL; property = property->NextSiblingElement()) {
					if(property->Value() == std::string("angleRange")) {
						double min, max;
						property->Attribute("min", &min);
						property->Attribute("max", &max);
						data.angleRange.setX(min);
						data.angleRange.setY(max);
					}
					if(property->Value() == std::string("rotSpeed")) {
						double rotSpeed;
						property->Attribute("value", &rotSpeed);
						data.rotSpeed = rotSpeed;
					}
					if(property->Value() == std::string("startSpeed")) {
						double startSpeed;
						property->Attribute("value", &startSpeed);
						data.startSpeed = startSpeed;
					}
					if(property->Value() == std::string("endSpeed")) {
						double endSpeed;
						property->Attribute("value", &endSpeed);
						data.endSpeed = endSpeed;
					}
					if(property->Value() == std::string("startSize")) {
						double startSize;
						property->Attribute("value", &startSize);
						data.startSize = startSize;
					}
					if(property->Value() == std::string("endSize")) {
						double endSize;
						property->Attribute("value", &endSize);
						data.endSize = endSize;
					}
					if(property->Value() == std::string("emitNumber")) {
						double emitNumber;
						property->Attribute("value", &emitNumber);
						data.emitNumber = emitNumber;
					}
					if(property->Value() == std::string("emitVariance")) {
						double emitVariance;
						property->Attribute("value", &emitVariance);
						data.emitVariance = emitVariance;
					}
					if(property->Value() == std::string("maxParticleLife")) {
						double maxParticleLife;
						property->Attribute("value", &maxParticleLife);
						data.maxParticleLife = maxParticleLife;
					}
					if(property->Value() == std::string("textureRect")) {
						int x, y, w, h;
						property->Attribute("x", &x);
						property->Attribute("y", &y);
						property->Attribute("w", &w);
						property->Attribute("h", &h);
						data.textureRect.x = x;
						data.textureRect.y = y;
						data.textureRect.w = w;
						data.textureRect.h = h;
					}
					if(property->Value() == std::string("startColor")) {
						int r, g, b, a;
						property->Attribute("r", &r);
						property->Attribute("g", &g);
						property->Attribute("b", &b);
						property->Attribute("a", &a);
						data.startColor.r = r;
						data.startColor.g = g;
						data.startColor.b = b;
						data.startColor.a = a;
					}
					if(property->Value() == std::string("endColor")) {
						int r, g, b, a;
						property->Attribute("r", &r);
						property->Attribute("g", &g);
						property->Attribute("b", &b);
						property->Attribute("a", &a);
						data.endColor.r = r;
						data.endColor.g = g;
						data.endColor.b = b;
						data.endColor.a = a;
					}
					if(property->Value() == std::string("blendMode")) {
						std::string blendMode = property->Attribute("mode");

						if (blendMode == "add")
							data.blendMode = SDL_BlendMode::SDL_BLENDMODE_ADD;
						else if (blendMode == "blend")
							data.blendMode = SDL_BlendMode::SDL_BLENDMODE_BLEND;
						else if (blendMode == "mod")
							data.blendMode = SDL_BlendMode::SDL_BLENDMODE_MOD;
						else if (blendMode == "none")
							data.blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
					}
					if(property->Value() == std::string("lifetime")) {
						double lifetime;
						property->Attribute("lifetime", &lifetime);
						data.lifetime = lifetime;
					}
				}
			}
        }

		Emitter* emitter = new Emitter(position, data);
		emittersList.push_back(emitter);
		return emitter;
	}

	void ParticleSystem::destroyEmitter(Emitter* emitter) {
		if (emitter != nullptr && !emittersList.empty()) {
			emittersToDestroy.push_back(emitter);
		}
	}

	void ParticleSystem::destroy() {
		if (!emittersList.empty()) {
			std::list<Emitter*>::const_iterator it;

			for (it = emittersList.begin(); it != emittersList.end(); ++it) {
				if (*it != nullptr)
					emittersToDestroy.push_back(*it);
			}
		}
	}

	void ParticleSystem::clean() {
		std::list<Emitter*>::const_iterator it;

		for (it = emittersList.begin(); it != emittersList.end(); ++it) {
			if (*it != nullptr)
				delete *it;
		}

		emittersList.clear();
		// TODO: Unload textres
	}

	void ParticleSystem::update(float deltaTime) {
		std::list<Emitter*>::const_iterator it;

		for (it = emittersList.begin(); it != emittersList.end(); ++it) {
			if (*it != nullptr)
				(*it)->update(deltaTime);
		}
	}

	void ParticleSystem::render(float deltaTime) {
		std::list<Emitter*>::const_iterator it;

		for (it = emittersList.begin(); it != emittersList.end(); ++it) {
			if (*it != nullptr)
				(*it)->render(deltaTime);
		}
	}

}
