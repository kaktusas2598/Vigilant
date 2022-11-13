#pragma once

#include "Window.hpp"
#include "InputManager.hpp"
#include "StateMachine.hpp"

#include <stdio.h>
#include <memory>
#include <vector>

// TODO: TEMP, DELETE
# include <SDL2/SDL_image.h>


namespace Vigilant {

	class StateMachine;
	class IGameState;
	class IEntity;

	/*! \brief IEngine
	 *         Main Engine Class.
	 *
	 */
	class IEngine {
		public:
			static IEngine* Instance() {
				if (s_pInstance == 0) {
					s_pInstance = new IEngine();
					return s_pInstance;
				}
				return s_pInstance;
			}
			/**
			 * init systems, create window and GL context
			 * @param title Title to be displayed on window title bar
			 * @param screenHeight Screen height in pixels
			 * @param screenWidth Screen width in pixels
			 * @param currentFlags window flags
			 * @param sdlEnabled Optional SDL rendering
			 * @sa WindowFlags
			 */
			void init(std::string title, int screenHeight, int screenWidth, unsigned int currentFlags, bool sdlRendering = false);
			void run(); ///< runs main application's loop
			void exit(); //< clean resources and exit application

			// void onInit();
			void addStates();
			void onExit();

			void handleEvents(SDL_Event& event);

			bool running(){ return m_isRunning; }
			void setRunning(bool running) { m_isRunning = running; }

			const float getFps() const { return m_fps; }
			
			StateMachine* getStateMachine() { return m_stateMachine.get(); }
			SDL_Renderer* getSDLRenderer() { return m_window.getSDLRenderer(); } ///<Optional SDL_Renderer getter

		private:
			IEngine();
			~IEngine();

			static IEngine* s_pInstance;

			void render(float deltaTime); ///< Main render method, renders current state
			void update(float deltaTime); ///< Main update method, sets different state or updates current one

			bool m_isRunning = false; ///< appliction running flag
			Window m_window; ///< main window instance

			float m_fps = 0; ///< main application's fps

			std::unique_ptr<StateMachine> m_stateMachine = nullptr; ///< state machine's instance
			IGameState* m_currentState = nullptr; ///< current state's instance

			std::vector<IEntity*> entities;

			bool SDLRenderingEnabled = false;
	};

	typedef IEngine TheEngine;
}
