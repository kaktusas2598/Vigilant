#pragma once

#include "Window.hpp"
#include "InputManager.hpp"

#include <stdio.h>

namespace Vigilant {

	class StateMachine;
	class IGameState;

	/*! \brief IEngine
	 *         Main Engine Class.
	 *
	 *  Application Launcher must extend from this class!
	 */
	class IEngine {
		public:

			IEngine();
			virtual ~IEngine();

			/**
			 * init systems, create window and GL context
			 * @param title Title to be displayed on window title bar
			 * @param screenHeight Screen height in pixels
			 * @param screenWidth Screen width in pixels
			 * @param currentFlags window flags
			 * @sa WindowFlags
			 */
			void init(std::string title, int screenHeight, int screenWidth, unsigned int currentFlags);
			void run(); ///< runs main application's loop
			void exit(); //< clean resources and exit application

			virtual void onInit() = 0;
			virtual void addStates() = 0;
			virtual void onExit() = 0;

			void handleEvents(SDL_Event& event);

			bool running(){ return m_isRunning; }

			const float getFps() const { return m_fps; }

			InputManager inputManager; ///< main application's input manager

		protected:

			void render(float deltaTime); ///< Main render method, renders current state
			void update(float deltaTime); ///< Main update method, sets different state or updates current one

			bool m_isRunning = false; ///< appliction running flag
			Window m_window; ///< main window instance

			float m_fps = 0; ///< main application's fps

			std::unique_ptr<StateMachine> m_stateMachine = nullptr; ///< state machine's instance
			IGameState* m_currentState = nullptr; ///< current state's instance

			int m_currentFrame;

	};
}
