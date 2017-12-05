#pragma once

#include "TextureCache.hpp"
#include <stdio.h>
#include <string>

namespace Vigilant {

	/* \brief Resource Manager Class.
	 * Holds textureCache object and is used to load textures.
	 */
	class ResourceManager
	{
		public:
			static GLTexture getTexture(std::string texturePath);

		private:
			static TextureCache m_textureCache;

	};

}
