#include "stdafx.h"
#include "msk-gl-shaderprogram.h"


MShaderProgram::MShaderProgram()
{
	m_hProgram = glCreateProgram();
}

MShaderProgram::~MShaderProgram()
{
	glDeleteProgram(m_hProgram);
}

void MShaderProgram::AttachShader(std::shared_ptr<MskShader>& shader)
{
	glAttachShader(m_hProgram, shader->GetId());

	// Increment the number of shaders we have associated with the program
	m_nShaderCount++;
}

void MShaderProgram::Link()
{
	if (m_nShaderCount >= 2) {
		// Perform the linking process
		glLinkProgram(m_hProgram);

		// Check the status
		GLint linkStatus;
		glGetProgramiv(m_hProgram, GL_LINK_STATUS, &linkStatus);

		if (linkStatus == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_hProgram, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = new char[maxLength];
			glGetProgramInfoLog(m_hProgram, maxLength, &maxLength, infoLog);

			// Output error
			OutputDebugString(infoLog);

			// The program is useless now. So delete it.
			glDeleteProgram(m_hProgram);

			std::cout << "Shader program linking failed." << std::endl;
		}
		else
		{
			std::cout << "Shader program linking OK." << std::endl;
		}
	}
	else
	{
		std::cout << "Can't link shaders - you need at least 2, but attached shader count is only: " << m_nShaderCount << std::endl;
	}
}

void MShaderProgram::Use()
{
	glUseProgram(m_hProgram);
}

void MShaderProgram::Disable()
{
	glUseProgram(0);
}

int MShaderProgram::AddAttribute(const std::string name)
{
	m_pAttributeLocations[name] = glGetAttribLocation(m_hProgram, name.c_str());
	return m_pAttributeLocations[name];
}

int MShaderProgram::AddUniform(const std::string name)
{
	m_pUniformLocations[name] = glGetUniformLocation(m_hProgram, name.c_str());
	return m_pUniformLocations[name];
}

void MShaderProgram::SetUniform(const std::string name, float x, float y, float z)
{
	glUniform3f(m_pUniformLocations[name], x, y, z);
}

void MShaderProgram::SetUniform(const std::string name, const glm::vec3 & v)
{
	glUniform3fv(m_pUniformLocations[name], 1, glm::value_ptr(v));
}

void MShaderProgram::SetUniform(const std::string name, const glm::vec4 & v)
{
	glUniform4fv(m_pUniformLocations[name], 1, glm::value_ptr(v));
}

void MShaderProgram::SetUniform(const std::string name, const glm::mat3 & m)
{
	glUniformMatrix3fv(m_pUniformLocations[name], 1, GL_FALSE, glm::value_ptr(m));
}

void MShaderProgram::SetUniform(const std::string name, const glm::mat4 & m)
{
	glUniformMatrix4fv(m_pUniformLocations[name], 1, GL_FALSE, glm::value_ptr(m));
}

void MShaderProgram::SetUniform(const std::string name, float value)
{
	glUniform1f(m_pUniformLocations[name], value);
}

void MShaderProgram::SetUniform(const std::string name, int value)
{
	glUniform1i(m_pUniformLocations[name], value);
}

void MShaderProgram::SetUniform(const std::string name, bool value)
{
	glUniform1i(m_pUniformLocations[name], value);
}
