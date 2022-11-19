#include "IEngine.hpp"
#include "FrameLimiter.hpp"
#include "ErrorHandler.hpp"
// #include "StateMachine.hpp"
#include "IGameState.hpp"
#include "MainMenuState.hpp"
#include "PlayState.hpp"
#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "MenuButton.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

#include "Coordinator.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "CollisionSystem.hpp"
#include "InputSystem.hpp"
#include <string>

namespace Vigilant {

	IEngine* IEngine::s_pInstance = nullptr;

	IEngine::IEngine()
	{
		// Initialize State Machine
		m_stateMachine = std::make_unique<StateMachine>(this);
	}

	IEngine::~IEngine()
	{
	}

	void IEngine::addStates() {
		MainMenuState *menuState = new MainMenuState();
		PlayState *playState = new PlayState();
		//Currently states is std:vector and each time they are just pushed to it in order
		m_stateMachine->addState(menuState);
		m_stateMachine->addState(playState);
		m_stateMachine->setState(0);
		return;
	}


	void IEngine::init(std::string title, int height, int width, unsigned int currentFlags, bool sdlEnabled){
		screenHeight = height;
		screenWidth = width;

		// Temporary test for audio - button audio
		TheSoundManager::Instance()->load("assets/button.wav","button", SOUND_SFX);

		// Initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			exitWithError("Could Not Initialize SDL");
		}

		// set up a double buffered window (minimizes flickering)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);//?

		SDLRenderingEnabled = sdlEnabled;
		m_window.setSDLRendering(SDLRenderingEnabled);
		m_window.create(title, screenHeight, screenWidth, currentFlags);
		
		if (SDLRenderingEnabled) {
			TheTextureManager::Instance()->setRenderer(m_window.getSDLRenderer());
		}

		TheEntityFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
		TheEntityFactory::Instance()->registerType("Player", new PlayerCreator());
		TheEntityFactory::Instance()->registerType("Enemy", new EnemyCreator());
		
		// ECS
		TheCoordinator::Instance()->init();
		// Register components in ECS so they all get IDs
		TheCoordinator::Instance()->registerComponent<Transform>();
		TheCoordinator::Instance()->registerComponent<RigidBody>();
		TheCoordinator::Instance()->registerComponent<Gravity>();
		TheCoordinator::Instance()->registerComponent<Sprite>();
		TheCoordinator::Instance()->registerComponent<Playable>();

		// Set correct components for each system so that correct entities end up being checked
		physicsSystem = TheCoordinator::Instance()->registerSystem<PhysicsSystem>(); {
			Signature signature;
			signature.set(TheCoordinator::Instance()->getComponentType<Gravity>());
			signature.set(TheCoordinator::Instance()->getComponentType<RigidBody>());
			signature.set(TheCoordinator::Instance()->getComponentType<Transform>());
			TheCoordinator::Instance()->setSystemSignature<PhysicsSystem>(signature);
		}
		
		renderSystem = TheCoordinator::Instance()->registerSystem<RenderSystem>(); {
			Signature signature;
			signature.set(TheCoordinator::Instance()->getComponentType<Sprite>());
			signature.set(TheCoordinator::Instance()->getComponentType<Transform>());
			TheCoordinator::Instance()->setSystemSignature<RenderSystem>(signature);
		}
		
		collisionSystem = TheCoordinator::Instance()->registerSystem<CollisionSystem>();  {
			Signature signature;
			signature.set(TheCoordinator::Instance()->getComponentType<Sprite>());
			signature.set(TheCoordinator::Instance()->getComponentType<Transform>());
			signature.set(TheCoordinator::Instance()->getComponentType<RigidBody>());
			TheCoordinator::Instance()->setSystemSignature<CollisionSystem>(signature);
		}

		inputSystem = TheCoordinator::Instance()->registerSystem<InputSystem>(); {
			Signature signature;
			signature.set(TheCoordinator::Instance()->getComponentType<Transform>());
			signature.set(TheCoordinator::Instance()->getComponentType<Playable>());
			signature.set(TheCoordinator::Instance()->getComponentType<RigidBody>());
			TheCoordinator::Instance()->setSystemSignature<InputSystem>(signature);
		}

		// TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
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
			// m_stateMachine->getCurrentState()->draw(deltaTime);
			if (m_currentState && m_currentState->getScreenState() == ScreenState::RUNNING) {
				m_currentState->draw(deltaTime);
			}

			// renderSystem->render();
			
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
	void IEngine::update(float deltaTime){

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
	void IEngine::handleEvents(SDL_Event& event)
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
