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

        if (TheInputManager::Instance()->isKeyPressed(SDLK_ESCAPE)) {
            TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
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
