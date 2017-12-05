#pragma once

#include <OpenGL/gl3.h>
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
