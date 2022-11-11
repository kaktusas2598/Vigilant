#include <stdio.h>
#include <SDL2/SDL.h>
//TODO: learn how to include headers properly - cause of so many issues and errors
#include "src/IEngine.hpp"
#include "src/IGameState.hpp"
#include "src/StateMachine.hpp"

const int WIDTH = 800, HEIGHT = 600;

class MenuState: public Vigilant::IGameState {
	public:
		int getNextStateIndex() const { return 1; }
		int getPrevStateIndex() const { return 1; }
		void build() {}
		void destroy() {}
		void onEntry() {
			printf("entering Menu State");
		}
		void onExit() {
			printf("exiting Menu State");
		}
		void update(float deltaTime) {
			m_game->inputManager.update();
			if (m_game->inputManager.isKeyDown(SDL_SCANCODE_RETURN)) {
				printf("key pressed");
				m_game->getStateMachine()->setState(1);
			}
		}
		void draw(float deltaTime) {}
	private:
};

class PlayState: public Vigilant::IGameState {
	public:
		int getNextStateIndex() const { return 0; }
		int getPrevStateIndex() const { return 0; }
		void build() {}
		void destroy() {}
		void onEntry() {
			printf("entering Play State");
		}
		void onExit() {
			printf("exiting Play State");
		}
		void update(float deltaTime) {
			// if (m_game->inputManager.isKeyDown(SDL_SCANCODE_RETURN)) {
				//m_currentScreenState = Vigilant::ScreenState::CHANGE_NEXT;
			// }
		}
		void draw(float deltaTime) {}
	private:
};

class MyGame : public Vigilant::IEngine {
	public:
		void onInit() { return; };
		void addStates() {
			MenuState *menuState = new MenuState();
			PlayState *playState = new PlayState();
			//Currently states is std:vector and each time they are just pushed to it in order
			m_stateMachine->addState(menuState);
			m_stateMachine->addState(playState);
			m_stateMachine->setState(0);
			return;
		}
		void onExit() { return; }
	private:
};

int main(int argc, char *argv[]) {
	
	printf("Initialising Engine...");		
	MyGame *engine = new MyGame();
	engine->init("Vigilant Engine Test", 640, 480, 0);
	engine->run();
	return 0;
 
}
