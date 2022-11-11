#include "GLSLProgram.hpp"
#include "ErrorHandler.hpp"
#include "IOManager.hpp"
#include <fstream>
#include <vector>
#include <memory>

namespace Vigilant {

	GLSLProgram::GLSLProgram() :
		m_numAttributes(0),
		m_programID(0),
		m_vertexShaderID(0),
		m_fragmentShaderID(0)
	{
		//empty
	}

	GLSLProgram::~GLSLProgram()
	{
		m_numAttributes = 0;
		m_programID = 0;
		m_vertexShaderID = 0;
		m_fragmentShaderID = 0;
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{

		std::string vertSource;
		std::string fragSource;

		IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);

		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());

	}

	///TODO: create better detailed debug message
	void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource)
	{
		//the vertex and fragment shaders are compiled, now we link them together
		//into a program

		//get a program object
		m_programID = glCreateProgram();

		// create the vertex shader
		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0)
		{
			exitWithError("Vertex Shader failed to be created");
		}

		// create the fragment shader
		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0)
		{
			exitWithError("Fragment Shader failed to be created");
		}

		//compile the shaders
		compileShader(vertexSource, "VERTEX SHADER", m_vertexShaderID);
		compileShader(fragmentSource, "FRAGMENT SHADER", m_fragmentShaderID);
	}

	void GLSLProgram::linkShaders()
	{
		// attach shaders to program
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		// link program
		glLinkProgram(m_programID);

		if (!checkStatus(m_programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS)) {

			glDetachShader(m_programID, m_vertexShaderID);
			glDetachShader(m_programID, m_fragmentShaderID);
			//        glDeleteProgram(m_programID);
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);

			exitWithError("Shaders failed to link!");
		}
	}

	void GLSLProgram::addAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(m_programID, m_numAttributes, attributeName.c_str());
		m_numAttributes++;
	}

	GLint GLSLProgram::getUniformLocation(const std::string &uniformName)
	{
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			exitWithError("Uniform " + uniformName + " not found in shader");
		}

		return location;
	}


	void GLSLProgram::use()
	{
		glUseProgram(m_programID);

		for(int i = 0; i < m_numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);

		for(int i = 0; i < m_numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLProgram::compileShader(const char* source, const std::string& name, GLuint id)
	{
		glShaderSource(id, 1, &source, nullptr);

		glCompileShader(id);

		if (!checkStatus(id, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS)) {
			// exit with faiure
			glDeleteShader(id);
			exitWithError("Shader " + name + " failed to compile");
		}
	}

	bool GLSLProgram::checkStatus(
			GLuint objectID,
			PFNGLGETSHADERIVPROC objectPropertyGetter,
			PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
			GLenum statusType)
	{
		GLint success = 0;
		objectPropertyGetter(objectID, statusType, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			objectPropertyGetter(objectID, GL_INFO_LOG_LENGTH, &maxLength);

			// the maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			getInfoLogFunc(objectID, maxLength, &maxLength, &errorLog[0]);

			std::printf("%s\n", &(errorLog[0]));

			return false;
		}
		return true;
	}


	void GLSLProgram::dispose()
	{
		if (m_programID) glDeleteProgram(m_programID);
		m_numAttributes = 0;
		m_programID = 0;
		m_vertexShaderID = 0;
		m_fragmentShaderID = 0;
	}
}
