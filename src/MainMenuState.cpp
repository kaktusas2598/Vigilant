#include "MainMenuState.hpp"

#include "StateParser.hpp"
#include "MenuButton.hpp"
#include "Engine.hpp"
#include "StateMachine.hpp"

// TODO: would be better to clear textures in base class
#include "TextureManager.hpp"

namespace Vigilant {
    const std::string MainMenuState::menuID = "MENU";

    void MainMenuState::onEntry() {
        StateParser stateParser;
        stateParser.parseState("state.xml", menuID, &gameEntities, &textureIDs,&soundIDs);

        m_callbacks.push_back(s_menuToPlay);
        m_callbacks.push_back(s_exitFromMenu);
        setCallbacks(m_callbacks);
    }

    void MainMenuState::onExit() {
        for(size_t i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->clean();
        }
        gameEntities.clear();

        for(size_t i = 0; i < textureIDs.size(); i++) {
            TheTextureManager::Instance()->clearFromTextureMap(textureIDs[i]);
        }
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

        // Testing text rendering
		// First apparent problem with this method from TextureManager:
		// No info about width and hight of rendered texture so no way to perfectly center this for example on any resolution
		TheTextureManager::Instance()->drawText("VIGILANT ENGINE", "arcade", 200, 0 , {255, 255, 255, 122}, 2);
    }

    void MainMenuState::setCallbacks(const std::vector<Callback>& callbacks) {
        for (size_t i = 0; i < gameEntities.size(); i++) {
            // if they are of type MenuButton then assign a callback based on the id passed in from the file
            if (dynamic_cast<MenuButton*>(gameEntities[i])) {
                MenuButton* pButton = dynamic_cast<MenuButton*>(gameEntities[i]);
                // Had to add -1 to fix callback indexing
                pButton->setCallback(callbacks[pButton->getCallbackID() - 1]);
            }
        }
    }

    void MainMenuState::s_menuToPlay() {
        TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
    }

    void MainMenuState::s_exitFromMenu() {
        TheEngine::Instance()->setRunning(false);
    }
}
