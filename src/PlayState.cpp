#include "PlayState.hpp"

#include "InputManager.hpp"
#include "IEngine.hpp"
#include "StateParser.hpp"
#include "LevelParser.hpp"
#include "StateMachine.hpp"
#include "TextureManager.hpp"

#include "ComponentTypes.hpp"
#include <random>
#include <vector>
#include "CollisionSystem.hpp"
#include "PhysicsSystem.hpp"

namespace Vigilant {

    const std::string PlayState::playID = "PLAY";

    void PlayState::update(float deltaTime) {
        level->update();
        // Level checks collisions against map and only after we update physics
        TheEngine::Instance()->getCollisionSystem()->checkMapCollision(level->getCollisionLayer());
        TheEngine::Instance()->getPhysicsSystem()->update(deltaTime);
        if (TheInputManager::Instance()->isKeyPressed(SDLK_ESCAPE)) {
            TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
        }
        
        // for (int i = 0; i < gameEntities.size(); i++) {
        //     gameEntities[i]->update(deltaTime);
        // }
        
        if (checkCollision(dynamic_cast<SDLEntity*>(gameEntities[0]), dynamic_cast<SDLEntity*>(gameEntities[1])))
        {
            TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
        }
    }

    void PlayState::draw(float deltaTime) {
        level->render();

        // for (int i = 0; i < gameEntities.size(); i++) {
        //     gameEntities[i]->draw(deltaTime);
        // }
    }

    void PlayState::onEntry() {
        LevelParser levelParser;
        level = levelParser.parseLevel("map.tmx");

        StateParser stateParser;
        stateParser.parseState("state.xml", playID, &gameEntities, &textureIDs);

        std::random_device rd;
		std::mt19937 rng{rd()}; // Initialize Mersenne Twister pseudo-random number generator
		std::uniform_int_distribution<int> distr(1, 50); // random numbers in range from 1 to 100
		for (std::size_t i = 0; i < 1000; ++i) {
			Entity entity = TheCoordinator::Instance()->createEntity();
			entities.push_back(entity);
			int randomNumber = distr(rng);
			TheCoordinator::Instance()->addComponent(entity, Transform{.x = (float)randomNumber, .y = 300});
			TheCoordinator::Instance()->addComponent(entity, Sprite{.width = 50, .height = 50, .textureID = "player", .numFrames = 12});
			TheCoordinator::Instance()->addComponent(entity, RigidBody{.velocityX = 1, .velocityY = (float)randomNumber, .accelerationX = 1, .accelerationY = 1});
			TheCoordinator::Instance()->addComponent(entity, Gravity{0, 8});
		}
    }

    void PlayState::onExit() {
        for (int i = 0; i < gameEntities.size(); i++) {
            gameEntities[i]->clean();
        }

        gameEntities.clear();

        for(int i = 0; i < textureIDs.size(); i++) {
            TheTextureManager::Instance()->clearFromTextureMap(textureIDs[i]);
        }
        for (int i = 0; i < entities.size(); i++) {
            TheCoordinator::Instance()->destoyEntity(entities[i]);
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