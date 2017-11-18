#pragma once

#include <OpenGL/gl3.h>
#include <iostream>
#include <stdio.h>
#include <string>

namespace Vigilant {

	class GLSLProgram
	{
		public:
			GLSLProgram();
			~GLSLProgram();

			void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

			void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

			void linkShaders();

			void addAttribute(const std::string& attributeName);

			GLint getUniformLocation(const std::string& uniformName);

			void use();
			void unuse();

			void dispose();

		private:

			void compileShader(const char* source, const std::string& name, GLuint id);

			int m_numAttributes = 0;

			GLuint m_programID = 0;
			GLuint m_vertexShaderID = 0;
			GLuint m_fragmentShaderID = 0;

	};
}
