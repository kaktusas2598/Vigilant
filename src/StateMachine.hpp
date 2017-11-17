#pragma once

#define STATE_NONE -1

#include <stdio.h>
#include <vector>

namespace Vigilant
{
	class IEngine;
	class IGameState;

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
			IEngine* m_game = nullptr;
			std::vector<IGameState*> m_states;
			int m_currentStateIndex = -1;
	};


}
