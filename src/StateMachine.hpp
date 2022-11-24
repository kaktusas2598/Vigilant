#ifndef __StateMachine__
#define __StateMachine__

#define STATE_NONE -1

#include <stdio.h>
#include <vector>

namespace Vigilant
{
	class Engine;
	class IGameState;

	/*! \brief StateMachine
	*         Finite State Machine Manager class.
	*
	* Finite State Machine. Manages std::vector of IGameState instances
	* Limitations: changes between states sequentually 
	*  @sa IGameState, Engine
	*/
	class StateMachine
	{
		public:
			StateMachine(Engine* game);
			~StateMachine();

			IGameState* moveNext();
			IGameState* movePrev();

			void setState(int nextStateIndex);
			void addState(IGameState* newState);

			void destroy();

			IGameState* getCurrentState();

		protected:
			Engine* m_game = nullptr; ///< Pointer to main engine's class
			std::vector<IGameState*> m_states; ///< Collection of states
			int m_currentStateIndex = -1; ///< Indicates index of current state in states collection, -1 indicates none
	};
}


#endif // __StateMachine__