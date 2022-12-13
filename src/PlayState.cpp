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

#include "imgui/imgui.h"

// TEMP
#include "SpriteComponent.hpp"
#include "ButtonComponent.hpp"

namespace Vigilant {

	static bool layerVisibility[10] = {false};

    const std::string PlayState::playID = "PLAY";

    void PlayState::update(float deltaTime) {
        level->update();
        //TODO: Keep lua thread running? coroutine?
        // Level checks collisions against map and only after we update physics
        // TheEngine::Instance()->getCollisionSystem()->checkMapCollision(level->getCollisionLayer());
        if (TheInputManager::Instance()->isKeyPressed(SDLK_ESCAPE)) {
            TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
        }

        EntityManager::Instance()->refresh();
        EntityManager::Instance()->update(deltaTime);

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
        EntityManager::Instance()->render(deltaTime);

        int i = 0;
        for (auto it = level->getCollisionLayers()->begin(); it != level->getCollisionLayers()->end(); ++it) {
            // Seems like member needs to be static but that doesnt make sense for tile layer
            ImGui::Begin("Debug Log");
			ImGui::Checkbox("Show solids", &layerVisibility[i]);
            ImGui::End();
			// FIXME: Weird bug: collision layer visibility toggles, but turning on colission layer seems to mess up actual Collision logic
			// This is super strange because Collision class does not care about isVisible member?
			(*it)->setVisible(layerVisibility[i]);
            i++;
        }
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

        // Test button
        Entity* button = new Entity();
        // Keep button centered, need to be affecyed by camera
		button->transform->setX((TheEngine::Instance()->getScreenWidth() - 200)/2);
        button->addComponent<SpriteComponent>()->load("mainmenubutton", 200, 80);
		button->addComponent<ButtonComponent>()->load(1, s_exitToMenu);
        EntityManager::Instance()->addEntity(button);

    }

    void PlayState::onExit() {
        for(size_t i = 0; i < textureIDs.size(); i++) {
            TheTextureManager::Instance()->clearFromTextureMap(textureIDs[i]);
        }

        ScriptEngine::Instance()->close();
        TheEngine::Instance()->setLevel(nullptr);
    }
}
