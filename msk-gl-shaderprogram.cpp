#include "stdafx.h"
#include "msk-gl-shaderprogram.h"

using namespace TexGen;

TgShaderProgram::TgShaderProgram() :
	m_shaderCount(0),
	m_program(glCreateProgram())
{
}

TgShaderProgram::~TgShaderProgram()
{
	glDeleteProgram(m_program);
}

void TgShaderProgram::attachShader(const TgShader& shader)
{
	glAttachShader(m_program, shader.getId());

	// Increment the number of shaders we have associated with the program
	m_shaderCount++;
}

void TgShaderProgram::link()
{
	if (m_shaderCount >= 2) {
		// Perform the linking process
		glLinkProgram(m_program);

		// Check the status
		GLint linkStatus;
		glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);

		if (linkStatus == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = new char[maxLength];
			glGetProgramInfoLog(m_program, maxLength, &maxLength, infoLog);

			// Output error
			OutputDebugString(infoLog);

			// The program is useless now. So delete it.
			glDeleteProgram(m_program);

			std::cout << "Shader program linking failed." << std::endl;
		}
		else
		{
			std::cout << "Shader program linking OK." << std::endl;
		}
	}
	else
	{
		std::cout << "Can't link shaders - you need at least 2, but attached shader count is only: " << m_shaderCount << std::endl;
	}
}

void TgShaderProgram::use()
{
	glUseProgram(m_program);
}

void TgShaderProgram::disable()
{
	glUseProgram(0);
}

int TgShaderProgram::addAttribute(const std::string& name)
{
	m_attributeLocations[name] = glGetAttribLocation(m_program, name.c_str());
	return m_attributeLocations[name];
}

int TgShaderProgram::addUniform(const std::string& name)
{
	m_uniformLocations[name] = glGetUniformLocation(m_program, name.c_str());
	return m_uniformLocations[name];
}

void TgShaderProgram::setUniform(const std::string& name, float x, float y, float z)
{
	glUniform3f(m_uniformLocations[name], x, y, z);
}

void TgShaderProgram::setUniform(const std::string& name, const glm::vec3 & v)
{
	glUniform3fv(m_uniformLocations[name], 1, glm::value_ptr(v));
}

void TgShaderProgram::setUniform(const std::string& name, const glm::vec4 & v)
{
	glUniform4fv(m_uniformLocations[name], 1, glm::value_ptr(v));
}

void TgShaderProgram::setUniform(const std::string& name, const glm::mat3 & m)
{
	glUniformMatrix3fv(m_uniformLocations[name], 1, GL_FALSE, glm::value_ptr(m));
}

void TgShaderProgram::setUniform(const std::string& name, const glm::mat4 & m)
{
	glUniformMatrix4fv(m_uniformLocations[name], 1, GL_FALSE, glm::value_ptr(m));
}

void TgShaderProgram::setUniform(const std::string& name, float value)
{
	glUniform1f(m_uniformLocations[name], value);
}

void TgShaderProgram::setUniform(const std::string& name, int value)
{
	glUniform1i(m_uniformLocations[name], value);
}

void TgShaderProgram::setUniform(const std::string& name, bool value)
{
	glUniform1i(m_uniformLocations[name], value);
}