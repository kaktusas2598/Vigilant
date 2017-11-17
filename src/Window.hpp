#pragma once

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

namespace Vigilant {

	enum WindowFlags {INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4};

	class Window {
		public:
			Window(){}
			~Window(){}

			void create(std::string title, int screenHeight, int screenWidth, unsigned int currentFlags);

			void swapBuffer();

			int getScreenWidth() { return m_screenWidth; }
			int getScreenHeight() { return m_screenHeight; }
			SDL_Window* getSDLWindow() { return m_pWindow; }

		private:

			SDL_Window * m_pWindow;

			int m_screenWidth, m_screenHeight;
	};
}
