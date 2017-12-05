#pragma once

#include <stdio.h>
#include <map>
#include "GLTexture.h"

namespace Vigilant {

	class TextureCache
	{
		public:
			TextureCache();
			~TextureCache();

			/// Loads texture and returns it
			GLTexture getTexture(std::string texturePath);

		private:
			std::map<std::string, GLTexture> mm_textureMap;
	};
}
