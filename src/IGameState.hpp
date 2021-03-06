#pragma once

#include <stdio.h>

namespace Vigilant
{
	class IEngine;

	/**
	 * Enumeration of avialable states per IGameState
	 */
	enum class ScreenState
	{
		NONE,
		RUNNING, //< state is running
		EXIT_APPLICATION, //< exit application
		CHANGE_NEXT, ///< change to next screen
		CHANGE_PREVIOUS ///< change to previous screen
	};


	/** \brief IGameState
	*         Abstract State Class. Blueprint for all application states
	*
	*  Abstract State Class. Playable or Menu states must extend this class!
	*  @sa StateMachine
	*/
	class IGameState
	{

		public:
			friend class StateMachine;

			IGameState(){
				//empty
			}
			virtual ~IGameState(){
				//empty
			}

			virtual int getNextStateIndex() const = 0; ///< Return the index of the next screen when changing screens
			virtual int getPrevStateIndex() const = 0; ///< Return the index of the previous screen when changing screens

			/**
			 * Called at beginning of application.
			 * Will be custom for each inherited class.
			 */
			virtual void build() = 0;
			/**
			 * Called at end of application.
			 * Will be custom for each inherited class.
			 */
			virtual void destroy() = 0;

			/**
			 * Called when screen enters focus.
			 */
			virtual void onEntry() = 0;
			/**
			 * Called when screen exits focus.
			 */
			virtual void onExit() = 0;

			/// Called in game loop
			virtual void update(float deltaTime) = 0;
			virtual void draw(float deltaTime) = 0;

			/// Sets m_game to be the parent game
			void setParentGame(IEngine* game) { m_game = game; }
			/// Sets current game state to running
			void setRunning()
			{
				m_currentScreenState = ScreenState::RUNNING;
			}

			int getScreenIndex() { return m_screenIndex; } ///< Current screen index getter
			ScreenState getScreenState() { return m_currentScreenState; } ///< current ScreenState enum getter

		protected:

			ScreenState m_currentScreenState = ScreenState::NONE;
			IEngine* m_game = nullptr; ///< Pointer to Engine
			int m_screenIndex = -1;
	};
}
