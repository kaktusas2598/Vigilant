#include "IEngine.hpp"
#include "FrameLimiter.hpp"
#include "ErrorHandler.hpp"
#include "StateMachine.hpp"
#include "IGameState.hpp"
#include "TextureManager.hpp"
#include <string>

namespace Vigilant {

	IEngine::IEngine()
	{
		//Initialize State Machine
		m_stateMachine = std::make_unique<StateMachine>(this);
	}

	IEngine::~IEngine()
	{
	}


	void IEngine::init(std::string title, int screenHeight, int screenWidth, unsigned int currentFlags){

		// Initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			exitWithError("Could Not Initialize SDL");
		}

		// TODO: DELETE
		// SDL_Texture* m_pTexture;
		// SDL_Rect m_sourceRectangle;
		// SDL_Rect m_destinationRectangle;

		// set up a double buffered window (minimizes flickering)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);//?

		//TODO: this is temp
		m_window.setSDLRendering(SDLRenderingEnabled);

		m_window.create(title, screenHeight, screenWidth, currentFlags);

		//initialize the current game
		onInit();

		//initialize game screens and add them to the screenList
		addStates();

		//set the MainGame's current game screen
		m_currentState = m_stateMachine->getCurrentState();

		//initialize game screen elements
		m_currentState->onEntry();

		//TODO: delete
		if(!TheTextureManager::Instance()->load("assets/player.png", "animate", m_window.getSDLRenderer()))
		{
			exitWithError("Could not load image");
		}

		//set the initial game screen to ScreenState::RUNNING
		m_currentState->setRunning();

		m_isRunning = true;//start main loop
	}

	void IEngine::run() {

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

		while (m_isRunning)
		{
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
			inputManager.update();

			//SEMI FIXED TIME STEP ??
			int updateCount = 0;
			while (totalDeltaTime > 0.0f && updateCount < MAX_PHYSICS_STEPS && m_isRunning)
			{
				// TODO: I added this adhoc
				SDL_Event event;
				while (SDL_PollEvent(&event)) {
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
	void IEngine::render(float deltaTime){

		if (SDLRenderingEnabled) {
			SDL_RenderClear(m_window.getSDLRenderer()); // clear the renderer to the draw color

			TheTextureManager::Instance()->draw("animate", 0,0, 128, 82,  m_window.getSDLRenderer());

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

		if (m_currentState && m_currentState->getScreenState() == ScreenState::RUNNING)
		{
			m_currentState->draw(deltaTime);
		}
	}

	/**
	* Update current state or change state if needed
	* @param deltaTime
	* @sa StateMachine
	*/
	void IEngine::update(float deltaTime){

		if (m_currentState)
		{
			switch (m_currentState->getScreenState())
			{
				//update the current running screen
				case ScreenState::RUNNING:
					m_currentState->update(deltaTime);
					break;
					//change to next screen
				case ScreenState::CHANGE_NEXT:
					//clean up running screen
					m_currentState->onExit();
					m_currentState = m_stateMachine->moveNext();
					if (m_currentState)
					{
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
					if (m_currentState)
					{
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
	void IEngine::handleEvents(SDL_Event& event)
	{
		//set the event type
		inputManager.setEventType(event.type);
		switch (event.type) {
			case SDL_QUIT:
				exit();
				break;
			case SDL_MOUSEMOTION:
				inputManager.setMouseCoords((float)event.motion.x, (float)event.motion.y);
				break;
			case SDL_KEYDOWN:
				inputManager.pressKey(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				inputManager.releaseKey(event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				inputManager.pressKey(event.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				inputManager.releaseKey(event.button.button);
				break;
			case SDL_TEXTINPUT:
				inputManager.addInputCharacters(event.text.text);
				break;
			case SDL_MOUSEWHEEL:
				if (event.wheel.y > 0)
					inputManager.setMouseWheel(1);
				if (event.wheel.y < 0)
					inputManager.setMouseWheel(-1);
				break;
			case (SDLK_ESCAPE):
				{
					//empty
				}
			default:
				break;
		}
	}

	void IEngine::exit(){

		std::cout << "Cleaning game..\n";
		if (SDLRenderingEnabled) {
			SDL_DestroyWindow(m_window.getSDLWindow());
			SDL_DestroyRenderer(m_window.getSDLRenderer());
			SDL_Quit();
		} else {

		}

		m_currentState->onExit();

		if (m_stateMachine)
		{
			m_stateMachine->destroy();
			m_stateMachine.reset();
		}

		m_isRunning = false;
	}
}
