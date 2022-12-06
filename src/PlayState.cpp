#include "PlayState.hpp"

#include "InputManager.hpp"
#include "Engine.hpp"
#include "StateParser.hpp"
#include "LevelParser.hpp"
#include "StateMachine.hpp"
#include "TextureManager.hpp"
#include <vector>

#include "Collision.hpp"

#include "imgui/imgui.h"

namespace Vigilant {

    const std::string PlayState::playID = "PLAY";

    void PlayState::update(float deltaTime) {
        level->update();
        //TODO: Keep lua thread running? coroutine?
        // Level checks collisions against map and only after we update physics
        // TheEngine::Instance()->getCollisionSystem()->checkMapCollision(level->getCollisionLayer());
        if (TheInputManager::Instance()->isKeyPressed(SDLK_ESCAPE)) {
            TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
        }

        for (size_t i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->update(deltaTime);

            Collision::checkMapCollision(gameEntities[i], level->getCollidableLayers());
        }

        // Find player and update camera
        for (size_t i = 0; i < gameEntities.size(); i++) {
            auto isPlayer = gameEntities[i]->getComponent<InputComponent>();
            if (isPlayer) {
                TheEngine::Instance()->camera.x = gameEntities[i]->transform->getX() - TheEngine::Instance()->camera.w/2;
                TheEngine::Instance()->camera.y = gameEntities[i]->transform->getY() - TheEngine::Instance()->camera.h/2;
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

        // if (checkCollision(dynamic_cast<SDLEntity*>(gameEntities[0]), dynamic_cast<SDLEntity*>(gameEntities[1]))) {
            // TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
        // }
    }

    void PlayState::draw(float deltaTime) {
        level->render();
        for (size_t i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->draw(deltaTime);
        }
        static bool layerVisibility[10] = {false};
        int i = 0;
        for (auto it = level->getCollisionLayers()->begin(); it != level->getCollisionLayers()->end(); ++it) {
            // Seems like member needs to be static but that doesnt make sense for tile layer
            ImGui::Begin("Debug Log");
			ImGui::Checkbox("Show solids", &layerVisibility[i]);
            ImGui::End();
			(*it)->setVisible(layerVisibility[i]);
            i++;
        }
    }

    void PlayState::onEntry() {
        LevelParser levelParser;
        // TODO: implement level loading without hardcoding name
        level = levelParser.parseLevel("map.tmx");

        StateParser stateParser;
        stateParser.parseState("state.xml", playID, &gameEntities, &textureIDs, &soundIDs);
    }

    void PlayState::onExit() {
        for (size_t i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->clean();
        }

        gameEntities.clear();

        for(size_t i = 0; i < textureIDs.size(); i++) {
            TheTextureManager::Instance()->clearFromTextureMap(textureIDs[i]);
        }
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
