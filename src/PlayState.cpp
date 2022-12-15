#include "PlayState.hpp"

#include "InputManager.hpp"
#include "Engine.hpp"
#include "StateParser.hpp"
#include "LevelParser.hpp"
#include "StateMachine.hpp"
#include "TextureManager.hpp"
#include "EntityManager.hpp"
#include <vector>

#include "Collision.hpp"

namespace Vigilant {

    const std::string PlayState::playID = "PLAY";

    void PlayState::update(float deltaTime) {
        level->update();

        // Level checks collisions against map and only after we update physics
        // TheEngine::Instance()->getCollisionSystem()->checkMapCollision(level->getCollisionLayer());
        if (TheInputManager::Instance()->isKeyPressed(SDLK_ESCAPE)) {
            TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
        }

        for (auto& e: EntityManager::Instance()->getEntities()) {
            // Chek each entity for collision against map tiles
            Collision::checkMapCollision(e, level->getCollidableLayers());

			// Find player and update camera
			auto isPlayer = e->getComponent<InputComponent>();
            if (isPlayer) {
				// Don't like this much looping.
				// Also need to decide if EntityManager good enough to be used everywhere or not
				//Collision::checkPlayerEntityCollision(e.get(), gameEntities);
				// Causes big lag!! Tried reducing number of entities to around 50 but it still is a problem
				Collision::checkPlayerEntityCollision(e, EntityManager::Instance()->getEntities());

                TheEngine::Instance()->camera.x = e->transform->getX() - TheEngine::Instance()->camera.w/2;
                TheEngine::Instance()->camera.y = e->transform->getY() - TheEngine::Instance()->camera.h/2;
            }
		}

		// Make sure camera doesn't go out of bounds of map
        if (TheEngine::Instance()->camera.x < 0) {
            TheEngine::Instance()->camera.x = 0;
        }

        if (TheEngine::Instance()->camera.y < 0) {
            TheEngine::Instance()->camera.y = 0;
        }

        //if (TheEngine::Instance()->camera.x > TheEngine::Instance()->camera.w) {
        if (TheEngine::Instance()->camera.x > (level->getWidth() - TheEngine::Instance()->camera.w)) {
            //TheEngine::Instance()->camera.x = TheEngine::Instance()->camera.w;
            TheEngine::Instance()->camera.x = level->getWidth() - TheEngine::Instance()->camera.w;
        }

        //if (TheEngine::Instance()->camera.y > TheEngine::Instance()->camera.h) {
		if (TheEngine::Instance()->camera.y > (level->getHeight() - TheEngine::Instance()->camera.h)) {
            //TheEngine::Instance()->camera.y = TheEngine::Instance()->camera.h;
            TheEngine::Instance()->camera.y = level->getHeight() - TheEngine::Instance()->camera.h;
        }
    }

    void PlayState::draw(float deltaTime) {
        level->render();
    }

    void PlayState::onEntry() {
        LevelParser levelParser;
        // TODO: implement level loading without hardcoding name
        level = levelParser.parseLevel("map.tmx");
        TheEngine::Instance()->setLevel(level);

        StateParser stateParser;
        // TEMP var, will probably want to remove it and only use EntityManager??
        std::vector<Entity*> gameEntities;
        stateParser.parseState("state.xml", playID, &gameEntities, &textureIDs, &soundIDs);
    }

    void PlayState::onExit() {
        for(size_t i = 0; i < textureIDs.size(); i++) {
            TheTextureManager::Instance()->clearFromTextureMap(textureIDs[i]);
        }

        ScriptEngine::Instance()->close();
        TheEngine::Instance()->setLevel(nullptr);
    }
}
