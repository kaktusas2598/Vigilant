#pragma once

#include "Window.hpp"
#include "InputManager.hpp"

#include <stdio.h>

namespace Vigilant {

	class StateMachine;
	class IGameState;

	class IEngine {
		public:

			IEngine();
			virtual ~IEngine();

			void init(std::string title, int screenHeight, int screenWidth, unsigned int currentFlags);
			void run();
			void exit();

			virtual void onInit() = 0;
			virtual void addStates() = 0;
			virtual void onExit() = 0;

			// Handles all sdl events
			void handleEvents(SDL_Event& event);

			bool running(){ return m_isRunning; }

			const float getFps() const { return m_fps; }

			InputManager inputManager; ///< main game's input manager

		protected:

			void render(float deltaTime);
			void update(float deltaTime);

			bool m_isRunning = false;
			Window m_window; ///< main game's window

			float m_fps = 0; ///< main game's fps

			std::unique_ptr<StateMachine> m_stateMachine = nullptr;
			IGameState* m_currentState = nullptr;

			int m_currentFrame;

	};
}
