#include "Application.h"

#include "Display.h"
#include "States/PlayState.h"

Application::Application()
{
	pushState(std::make_unique<State::PlayState>(*this));
}

void Application::run()
{
	while (Display::isOpen())
	{
		Display::clear();

		m_states.top()->input();
		m_states.top()->update();
		m_states.top()->draw();

		Display::update();

		Display::checkForClose();
	}
}

void Application::pushState(std::unique_ptr<State::GameState>state)
{
	m_states.push(std::move(state));
}

void Application::popState()
{
	m_states.pop();
}
