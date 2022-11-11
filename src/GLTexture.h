#pragma once

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#include <string>

namespace Vigilant {

	/* \brief OpenGL Texture Structure
	 * Structure maintaining main information about texture
	 */
	struct GLTexture
	{
		GLuint id;
		int width;
		int height;
		std::string filePath;
	};

}
