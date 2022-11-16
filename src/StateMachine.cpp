#include "StateMachine.hpp"
#include "IGameState.hpp"
#include "IEngine.hpp"

namespace Vigilant {

	StateMachine::StateMachine(IEngine* game) :
		m_game(game)
	{
		//empty
	}

	StateMachine::~StateMachine()
	{
		destroy();
	}

	IGameState* StateMachine::moveNext()
	{
		IGameState* currentState = getCurrentState();

		if (currentState->getNextStateIndex() != STATE_NONE)
		{
			m_currentStateIndex = currentState->getNextStateIndex();
		}
		return getCurrentState();
	}

	IGameState* StateMachine::movePrev()
	{
		IGameState* currentState = getCurrentState();

		if (currentState->getPrevStateIndex() != STATE_NONE)
		{
			m_currentStateIndex = currentState->getPrevStateIndex();
		}
		return getCurrentState();
	}

	void StateMachine::setState(int nextScreenIndex)
	{
		m_currentStateIndex = nextScreenIndex;
	}

	void StateMachine::addState(IGameState* newScreen)
	{
		//TODO: This is shit, I think this was causing segfault, I need to replace order of consts in States.hpp
		//It seems like this is just assigning them in order - poor design
		newScreen->m_screenIndex = (int)m_states.size();

		//add new screen to vector of screens
		m_states.push_back(newScreen);

		//set up the new screen
		newScreen->build();
		newScreen->setParentGame(m_game);
	}

	void StateMachine::destroy()
	{
		//destroy the entire screenList
		for (size_t i = 0; i < m_states.size(); i++)
		{
			m_states[i]->destroy();
		}

		//no screens left
		m_states.resize(0);
		m_currentStateIndex = STATE_NONE;
	}

	IGameState* StateMachine::getCurrentState()
	{
		//error check
		//FIXME: why did I do this? if no state provided, engine will segfault
		if(m_currentStateIndex == STATE_NONE) {
			return nullptr;
		}
	

		return m_states[m_currentStateIndex];
	}


}
