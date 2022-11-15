#include "Window.hpp"
#include "ErrorHandler.hpp"

namespace Vigilant {

	void Window::create(std::string title, int screenHeight, int screenWidth, unsigned int currentFlags){

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		Uint32 flags = 0;
		// SDL will use OpenGL for rendering
		if (GLContextEnabled) {
			flags = SDL_WINDOW_OPENGL;
		}
		
		// Alternative way to do rendering would be to use SDL_Renderer: that way we don't need to use
		// SDL_GL_CreateContext(), but are effectively dropping OpenGL support
		// TODO: Consider implementing alternative rendering options: SDL

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
		m_pWindow = SDL_CreateWindow(title.c_str() , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (m_pWindow == nullptr)
		{
			exitWithError("SDL Window could not be created :(");
		}

		if (GLContextEnabled) {
			// set up OpenGL context
			SDL_GLContext glContext = SDL_GL_CreateContext(m_pWindow);
			if (glContext == nullptr)
			{
				exitWithError("SDL_GL context could not be created");
			}

			GLenum glewStatus = glewInit();
			if (glewStatus != GLEW_OK) {
				exitWithError("Failed to initialise GLEW");
			}
		} else {
			// Set up simpler pure 2D SDL Renderer
			renderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (renderer == 0) {
				exitWithError("SDL_Renderer could not be created");
			}
		}

		

		//TODO: this seems out of place, gl vertex arrays should probably go to rendering
		// bind a VAO
        GLuint vertexArrayID;
		//Segfault here somewhere
        //glGenVertexArrays(1, &vertexArrayID);
        //glBindVertexArray(vertexArrayID);

        //check the OpenGL version
        printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));

		if (GLContextEnabled) {
			// set background color
			glClearColor(255.0f, 0.0f, 0.0f, 1.0f);

			//set VSYNC
			SDL_GL_SetSwapInterval(0);

			//enable alpha blending
			glEnable(GL_BLEND);
			//what kind of blending we want
			//in this case, we want alpha 0 to be transparent
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			SDL_SetRenderDrawColor(renderer, 255,0,0,255);
		}
	}

	void Window::swapBuffer()
	{
		if (GLContextEnabled) {
			//swap buffer and draw everything to the screen
			SDL_GL_SwapWindow(m_pWindow);
		}
	}

}
