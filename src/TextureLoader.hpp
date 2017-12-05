#pragma once

#include <stdio.h>
#include <string>
#include "GLTexture.h"

namespace Vigilant {

	/* \brief TextureLoader class
	 * TODO: is this good pattern for this task?
	 */
	class TextureLoader
	{
		public:
			static GLTexture load(std::string filePath);
	};
}
