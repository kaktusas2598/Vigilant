#include "GameState.hpp"

#include "Engine.hpp"
#include "StateParser.hpp"
#include "LevelParser.hpp"
#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "EntityManager.hpp"
#include "ScriptEngine.hpp"
#include "Logger.hpp"

namespace Vigilant {

    void GameState::onEntry() {
        // Allocate on stack one time utilities for state loading
        StateParser stateParser;
        LevelParser levelParser;
        std::string map;
        stateParser.parseState("state.xml", stateID, &textureIDs, &soundIDs);
        map = stateParser.getMapName("state.xml", stateID);

        if (!map.empty()) {
            Logger::Instance()->info("Loading level.");
            level = levelParser.parseLevel(map.c_str());
            TheEngine::Instance()->setLevel(level);
        }
    }

    void GameState::onExit() {
        for(size_t i = 0; i < textureIDs.size(); i++) {
            TheTextureManager::Instance()->clearFromTextureMap(textureIDs[i]);
        }

        SoundManager::Instance()->cleanSoundMaps();

        ScriptEngine::Instance()->close();

        EntityManager::Instance()->clean();

        if (level != nullptr) {
            for (auto id : level->getTextureIDs())
                TheTextureManager::Instance()->clearFromTextureMap(id);
            delete level;
            TheEngine::Instance()->setLevel(nullptr);
        }
    }

    void GameState::update(float deltaTime) {
        if (level != nullptr)
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

        int xMax = (level == nullptr) ? TheEngine::Instance()->camera.w : (level->getWidth() - TheEngine::Instance()->camera.w);
        int yMax = (level == nullptr) ? TheEngine::Instance()->camera.h : (level->getHeight() - TheEngine::Instance()->camera.h);

        if (TheEngine::Instance()->camera.x > xMax) {
            TheEngine::Instance()->camera.x = xMax;
        }
        if (TheEngine::Instance()->camera.y > yMax) {
            TheEngine::Instance()->camera.y = yMax;
        }
    }

}
