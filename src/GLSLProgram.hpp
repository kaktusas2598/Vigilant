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
#include <GL/glu.h>
#include <GL/glut.h>
#endif
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

			//Function pointer magic for refactoring compile and link errors checking
			bool checkStatus(
					GLuint objectID,
					PFNGLGETSHADERIVPROC objectPropertyGetter,
					PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
					GLenum statusType);

			int m_numAttributes = 0;

			GLuint m_programID = 0;
			GLuint m_vertexShaderID = 0;
			GLuint m_fragmentShaderID = 0;

	};
}
