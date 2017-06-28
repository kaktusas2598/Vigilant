#pragma once

//#include <stack>
#include <memory>

#include "GameState.h"

class Application
{
	public:
		Application();

		void run();

		void pushState(std::unique_ptr<State::GameState>);
		void popState();

	private:
		std::stack<std::unique_ptr<State::GameState>> mStates;

};
#endif
