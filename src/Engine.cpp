#include "Engine.hpp"
#include "FrameLimiter.hpp"
#include "ErrorHandler.hpp"
#include "TextureManager.hpp"

#include "IGameState.hpp"
#include "MainMenuState.hpp"
#include "PlayState.hpp"

#include "MenuButton.hpp"
#include "ScrollingBackground.hpp"

#include <string>
#include "Logger.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_sdlrenderer.h"

namespace Vigilant {

	Engine* Engine::s_pInstance = nullptr;

	Engine::Engine()
	{
		// Initialize State Machine
		Logger::Instance()->info("Initialising the engine.");
		m_stateMachine = std::make_unique<StateMachine>(this);
	}

	Engine::~Engine()
	{
	}

	void Engine::addStates() {
		Logger::Instance()->info("Adding states.");
		MainMenuState *menuState = new MainMenuState();
		PlayState *playState = new PlayState();
		//Currently states is std:vector and each time they are just pushed to it in order
		m_stateMachine->addState(menuState);
		m_stateMachine->addState(playState);
		m_stateMachine->setState(0);
		return;
	}


	void Engine::init(std::string title, int height, int width, unsigned int currentFlags, bool sdlEnabled){
		screenHeight = height;
		screenWidth = width;

		// Initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			exitWithError("Could Not Initialize SDL.");
		}

		// set up a double buffered window (minimizes flickering)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);//?

		SDLRenderingEnabled = sdlEnabled;
		m_window.setSDLRendering(SDLRenderingEnabled);
		m_window.create(title, screenHeight, screenWidth, currentFlags);

		if (SDLRenderingEnabled) {
			Logger::Instance()->info("Setting up SDL renderer.");
			TheTextureManager::Instance()->setRenderer(m_window.getSDLRenderer());
		}

		// TODO: get rid of these and implement using ECS
		TheEntityFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
		TheEntityFactory::Instance()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());

		//initialize the current game
		// onInit();

		//initialize game screens and add them to the screenList
		addStates();

		//set the MainGame's current game screen
		m_currentState = m_stateMachine->getCurrentState();

		//initialize game screen elements
		m_currentState->onEntry();
		//set the initial game screen to ScreenState::RUNNING
		m_currentState->setRunning();

		m_isRunning = true;//start main loop
	}

	void Engine::run() {

		const float DESIRED_FPS = 60.0f;
		//maximum steps, used to avoid the 'spiral of death'
		const int MAX_PHYSICS_STEPS = 6;
		//set max delta time to avoid speedup
		const float MAX_DELTA_TIME = 1.0f;
		//milliseconds per second
		const float MS_PER_SECOND = 1000;
		//the fps we want in ms / frame
		const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;

		//set max fps to 60
		FrameLimiter frameLimiter;
		frameLimiter.setMaxFPS(DESIRED_FPS);

		//starting time
		float prevTicks = SDL_GetTicks();

		while (m_isRunning) {
			frameLimiter.begin();

			//time at the start of the frame
			Uint32 newTicks = SDL_GetTicks();
			//total time the frame took
			Uint32 frameTime = SDL_GetTicks() - prevTicks;
			//update previous frame time
			prevTicks = newTicks;

			//how much extra time we have in the frame
			float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

			//update input manager
			TheInputManager::Instance()->update();

			//SEMI FIXED TIME STEP ??
			int updateCount = 0;
			while (totalDeltaTime > 0.0f && updateCount < MAX_PHYSICS_STEPS && m_isRunning) {
				SDL_Event event;
				while (SDL_PollEvent(&event)) {
					ImGui_ImplSDL2_ProcessEvent(&event);
					handleEvents(event);
				}

				//limit deltatime to 1.0 so no speedup (1.0 being one frame and .2 being a fifth of a frame)
				float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

				deltaTime = deltaTime / DESIRED_FPS;

				update(deltaTime);

				render(deltaTime);
				//std::cout << deltaTime << std::endl;
				//std::cout <<  " T: " << totalDeltaTime << std::endl;
				totalDeltaTime -= deltaTime;

				//if we have reached the maximum physics steps, just continue on with the frame
				updateCount++;
			}

			m_fps = frameLimiter.end();

			//swap window buffer for less flickering
			m_window.swapBuffer();
		}
	}

	/**
	* Renders current state
	* @param deltaTime
	*/
	void Engine::render(float deltaTime){

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Begin("Debug Log");                          // Create a window called "Hello, world!" and append into it.
		//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//ImGui::SameLine();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (SDLRenderingEnabled) {
			ImGui::Render();
			//SDL_SetRenderDrawColor(m_window.getSDLRenderer(), (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));

			SDL_RenderClear(m_window.getSDLRenderer()); // clear the renderer to the draw color
			// m_stateMachine->getCurrentState()->draw(deltaTime);
			if (m_currentState && m_currentState->getScreenState() == ScreenState::RUNNING) {
				m_currentState->draw(deltaTime);
			}
			ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
			SDL_RenderPresent(m_window.getSDLRenderer()); // draw to the screen
		} else {
			//TODO: below is temporary code to test OpenGL drawing
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Following is deprecated GL stuff
			glEnableClientState(GL_COLOR_ARRAY); //enable colouring
			glBegin(GL_TRIANGLES);
			glColor3f(0,0,0); //set colour for GL immediate mode(deprecated)
			glVertex2f(0, 0); //draw each vertice
			glVertex2f(0, 200);
			glVertex2f(500, 500);
			glEnd();
			///////////////////////
		}
	}

	/**
	* Update current state or change state if needed
	* @param deltaTime
	* @sa StateMachine
	*/
	void Engine::update(float deltaTime){

		if (m_currentState) {
			switch (m_currentState->getScreenState()) {
				//update the current running screen
				case ScreenState::RUNNING:
					m_currentState->update(deltaTime);
					break;
					//change to next screen
				case ScreenState::CHANGE_NEXT:
					//clean up running screen
					m_currentState->onExit();
					m_currentState = m_stateMachine->moveNext();
					if (m_currentState) {
						//initialize zanew running screen
						m_currentState->setRunning();
						m_currentState->onEntry();
					}
					break;
					//change to previous screen
				case ScreenState::CHANGE_PREVIOUS:
					//clean up running screen
					m_currentState->onExit();
					m_currentState = m_stateMachine->movePrev();
					if (m_currentState) {
						//initialize new running screen
						m_currentState->setRunning();
						m_currentState->onEntry();
					}
					break;
					//exit game
				case ScreenState::EXIT_APPLICATION:
					exit();
					break;

					// if NONE, do nothing
				default:
					break;

			}
		}
		else { exit(); }
	}

	/**
	* Takes SDL_Event reference from current running state and according to event types to InputManager.
	* @param event SDL_Event& structure
	* @sa IGameState::update, InputManager
	*/
	void Engine::handleEvents(SDL_Event& event)
	{
		//set the event type
		TheInputManager::Instance()->setEventType(event.type);
		switch (event.type) {
			case SDL_QUIT:
				exit();
				break;
			case SDL_MOUSEMOTION:
				TheInputManager::Instance()->setMouseCoords((float)event.motion.x, (float)event.motion.y);
				break;
			case SDL_KEYDOWN:
				TheInputManager::Instance()->pressKey(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				TheInputManager::Instance()->releaseKey(event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				TheInputManager::Instance()->pressKey(event.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				TheInputManager::Instance()->releaseKey(event.button.button);
				break;
			case SDL_TEXTINPUT:
				TheInputManager::Instance()->addInputCharacters(event.text.text);
				break;
			case SDL_MOUSEWHEEL:
				if (event.wheel.y > 0)
					TheInputManager::Instance()->setMouseWheel(1);
				if (event.wheel.y < 0)
					TheInputManager::Instance()->setMouseWheel(-1);
				break;
			case (SDLK_ESCAPE):
				{
					//empty
				}
			default:
				break;
		}
	}

	void Engine::exit(){

		m_currentState->onExit();

		if (m_stateMachine)
		{
			m_stateMachine->destroy();
			m_stateMachine.reset();
		}

		m_isRunning = false;

		ImGui_ImplSDLRenderer_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		if (SDLRenderingEnabled) {
			SDL_DestroyWindow(m_window.getSDLWindow());
			SDL_DestroyRenderer(m_window.getSDLRenderer());
			SDL_Quit();
		} else {
			// GL cleanup goes here
		}
	}
}