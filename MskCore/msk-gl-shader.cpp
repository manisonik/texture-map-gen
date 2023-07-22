#include "stdafx.h"
#include "msk-gl-shader.h"

using namespace Manisonik;

MShader::MShader(const MskShaderType & type)
{
	m_nId = glCreateShader(type);

	// Get the type of the shader
	if (type == GL_VERTEX_SHADER)
	{
		m_strType = "Vertex";
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		m_strType = "Fragment";
	}
	else
	{
		m_strType = "Geometry";
	}
}

MShader::~MShader() { }

GLuint MShader::GetId()
{
	return m_nId;
}

std::string MShader::GetSource()
{
	return m_strSource;
}

void MShader::LoadFromString(const std::string & source)
{
	// Keep hold of a copy of the source
	m_strSource = source;

	// Get the source as a pointer to an array of characters
	const char *sourceChars = m_strSource.c_str();

	// Associate the source with the shader id
	glShaderSource(m_nId, 1, &sourceChars, NULL);
}

void MShader::LoadFromFile(const std::string & filename)
{
	std::ifstream file;

	file.open(filename.c_str());

	if (!file.good())
	{
		std::cout << "Failed to open file: " << filename << std::endl;
		exit(-1);
	}

	// Create a string stream
	std::stringstream stream;

	// Dump the contents of the file into it
	stream << file.rdbuf();

	// Close the file
	file.close();

	// Convert the StringStream into a string
	m_strSource = stream.str();

	// Get the source string as a pointer to an array of characters
	const char* sourceChars = m_strSource.c_str() + '\0';

	// Associate the source with the shader id
	glShaderSource(m_nId, 1, &sourceChars, NULL);
}

void MShader::Compile()
{
	// Compile the shader
	glCompileShader(m_nId);

	// Check the compilation status and report any errors
	GLint shaderStatus;
	glGetShaderiv(m_nId, GL_COMPILE_STATUS, &shaderStatus);

	// If the shader failed to compile, display the info log and quit out
	if (shaderStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(m_nId, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(m_nId, infoLogLength, NULL, strInfoLog);

		// Output error
		OutputDebugString(strInfoLog);

		std::cout << m_strType << " shader compilation failed: " << strInfoLog << std::endl;
		delete[] strInfoLog;
	}
	else
	{
		std::cout << m_strType << " shader compilation OK" << std::endl;
	}
}

void MShader::GetError()
{
}