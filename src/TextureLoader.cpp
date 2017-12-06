#include "TextureLoader.hpp"
#include "ErrorHandler.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace Vigilant {

	GLTexture TextureLoader::load(std::string filePath) {

		//Initialize texture object to 0
		GLTexture texture = {};
		int width, height, nrChannels;
		//Load image
		unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (!data) {
			exitWithError("Failed to load texture!");
			//Free image memory
			stbi_image_free(data);
		}

		glGenTextures(1, &(texture.id));

		glBindTexture(GL_TEXTURE_2D, texture.id);

		//TODO: read more about this
		//Set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum glChannels;
		switch (nrChannels) {
			case 4:
				glChannels = GL_RGBA;
				break;
			case 3:
				glChannels = GL_RGB;
				break;
			default:
				std::cout << "nrChannels is: " << nrChannels << std::endl;
				glChannels = GL_RGB;
				break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, glChannels, width, height, 0, glChannels, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		//set texture dimensions
		texture.width = width;
		texture.height = height;
		texture.filePath = filePath;

		//Free image memory
		stbi_image_free(data);

		return texture;
	}
}
