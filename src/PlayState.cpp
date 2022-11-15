#include "PlayState.hpp"

#include "InputManager.hpp"
#include "IEngine.hpp"
#include "StateParser.hpp"
#include "LevelParser.hpp"
#include "StateMachine.hpp"

namespace Vigilant {

    const std::string PlayState::playID = "PLAY";

    void PlayState::update(float deltaTime) {
        level->update();

        if (TheInputManager::Instance()->isKeyPressed(SDLK_ESCAPE)) {
            TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
        }
        
        for (int i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->update(deltaTime);
        }
        
        if (checkCollision(dynamic_cast<SDLEntity*>(gameEntities[0]), dynamic_cast<SDLEntity*>(gameEntities[1])))
        {
            TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
        }
    }

    void PlayState::draw(float deltaTime) {
        level->render();

        for (int i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->draw(deltaTime);
        }
    }

    void PlayState::onEntry() {
        LevelParser levelParser;
        level = levelParser.parseLevel("map.tmx");

        StateParser stateParser;
        stateParser.parseState("state.xml", playID, &gameEntities, &textureIDs);
        // return true;
    }

    void PlayState::onExit() {
        for (int i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->clean();
        }

        gameEntities.clear();

        for(int i = 0; i < textureIDs.size(); i++) {
            //TODO : Implement texture manager clear from texture map method
            // TheTextureManager::Instance()->
        }
        // return true;
    }

    bool PlayState::checkCollision(SDLEntity *p1, SDLEntity* p2) {
        int leftA, leftB;
        int rightA, rightB;
        int topA, topB;
        int bottomA, bottomB;

        leftA = p1->getPosition().getX();
        rightA = p1->getPosition().getX() + p1->getWidth();
        topA = p1->getPosition().getY();
        bottomA = p1->getPosition().getY() + p1->getHeight();

        leftB = p2->getPosition().getX();
        rightB = p2->getPosition().getX() + p2->getWidth();
        topB = p2->getPosition().getY();
        bottomB = p2->getPosition().getY() + p2->getHeight();

        // If any of the sides from A are outside of B
        if (bottomA <= topB) { return false; }
        if (topA >= bottomB) { return false; }
        if (rightA <= leftB) { return false; }
        if (leftA >= rightB) { return false; }
        
        return true;
    }
}