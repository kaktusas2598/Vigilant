#pragma once

#define STATE_NONE -1

#include <stdio.h>
#include <vector>

namespace Vigilant
{
	class IEngine;
	class IGameState;

	/*! \brief StateMachine
	*         Finite State Machine Manager class.
	*
	*  Finite State Machine. Manages std::vector of IGameState instances
	*  @sa IGameState, IEngine
	*/
	class StateMachine
	{
		public:
			StateMachine(IEngine* game);
			~StateMachine();

			IGameState* moveNext();
			IGameState* movePrev();

			void setState(int nextStateIndex);
			void addState(IGameState* newState);

			void destroy();

			IGameState* getCurrentState();

		protected:
			IEngine* m_game = nullptr; ///< Pointer to main engine's class
			std::vector<IGameState*> m_states; ///< Collection of states
			int m_currentStateIndex = -1; ///< Indicates index of current state in states collection, -1 indicates none
	};
}
