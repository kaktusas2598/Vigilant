#include "Window.hpp"
#include "ErrorHandler.hpp"

namespace Vigilant {

	void Window::create(std::string title, int screenHeight, int screenWidth, unsigned int currentFlags){

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		//if succeed, create the window
		m_pWindow = SDL_CreateWindow(title.c_str() , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenHeight, screenWidth, flags);
		if (m_pWindow == nullptr)
		{
			exitWithError("SDL Window could not be created :(");
		}

		// set up OpenGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(m_pWindow);
		if (glContext == nullptr)
		{
			exitWithError("SDL_GL context could not be created");
		}

		// bind a VAO
        GLuint vertexArrayID;
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        //check the OpenGL version
        printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));

        // set background color
        glClearColor(255.0f, 255.0f, 255.0f, 1.0f);

        //set VSYNC
        SDL_GL_SetSwapInterval(0);

        //enable alpha blending
        glEnable(GL_BLEND);
        //what kind of blending we want
        //in this case, we want alpha 0 to be transparent
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//SDL_Surface * pTempSurface = SDL_LoadBMP("assets/animate.bmp");
		//SDL_Surface * pTempSurface = IMG_Load("assets/animate-alpha.png");

		//m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pTempSurface);

		//SDL_FreeSurface(pTempSurface);

		//SDL_QueryTexture(m_pTexture, NULL, NULL, &m_sourceRectangle.w, &m_sourceRectangle.h);
		//TextureManager::Instance()->load("assets/animate-alpha.png", "animate", m_pRenderer);
	}

	void Window::swapBuffer()
	{
		//swap buffer and draw everything to the screen
		SDL_GL_SwapWindow(m_pWindow);
	}

}
