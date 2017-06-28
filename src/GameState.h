#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stack>
#include <memory>

class Application;

namespace State
{
	class GameState
	{
		public:
			GameState(Application& application);

			virtual void input() = 0;
			virtual void update() = 0;
			virtual void draw() = 0;
		protected:
			Application* m_application;
	};
}
#endif
