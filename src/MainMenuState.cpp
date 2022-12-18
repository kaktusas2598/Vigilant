#include "MainMenuState.hpp"

#include "StateParser.hpp"
#include "Engine.hpp"
#include "StateMachine.hpp"

#include "ScriptEngine.hpp"
#include "EntityManager.hpp"

// TODO: would be better to clear textures in base class
#include "TextureManager.hpp"

// TODO: Refactor all of this, most of this stuff is exactly the same as in Play state except for:
// 1. Level
// 2. state ID string
namespace Vigilant {
    const std::string MainMenuState::menuID = "MENU";

    void MainMenuState::onEntry() {
        StateParser stateParser;
        stateParser.parseState("state.xml", menuID, &gameEntities, &textureIDs,&soundIDs);
    }

    void MainMenuState::onExit() {
        for(size_t i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->clean();
        }
        gameEntities.clear();

        for(size_t i = 0; i < textureIDs.size(); i++) {
            TheTextureManager::Instance()->clearFromTextureMap(textureIDs[i]);
        }

        ScriptEngine::Instance()->close();
        EntityManager::Instance()->clean();
    }

    void MainMenuState::update(float deltaTime) {
        for(size_t i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->update(deltaTime);
        }
    }

    void MainMenuState::draw(float deltaTime) {
        for(size_t i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->draw(deltaTime);
        }
    }
}
