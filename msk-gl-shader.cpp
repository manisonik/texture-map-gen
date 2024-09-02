#include "stdafx.h"
#include "msk-gl-shader.h"

using namespace TexGen;

TgShader::TgShader(const eShaderType& type)
{
	m_id = glCreateShader(type);

	// Get the type of the shader
	if (type == GL_VERTEX_SHADER)
	{
		m_type = "Vertex";
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		m_type = "Fragment";
	}
	else
	{
		m_type = "Geometry";
	}
}

TgShader::~TgShader() { }

GLuint TgShader::getId() const
{
	return m_id;
}

const std::string& TgShader::getSource() const
{
	return m_source;
}

void TgShader::loadFromString(const std::string & source)
{
	// Keep hold of a copy of the source
	m_source = source;

	// Get the source as a pointer to an array of characters
	const char *sourceChars = m_source.c_str();

	// Associate the source with the shader id
	glShaderSource(m_id, 1, &sourceChars, NULL);
}

void TgShader::loadFromFile(const std::string& filename)
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
	m_source = stream.str();

	// Get the source string as a pointer to an array of characters
	const char* sourceChars = m_source.c_str() + '\0';

	// Associate the source with the shader id
	glShaderSource(m_id, 1, &sourceChars, NULL);
}

void TgShader::compile()
{
	// Compile the shader
	glCompileShader(m_id);

	// Check the compilation status and report any errors
	GLint shaderStatus;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &shaderStatus);

	// If the shader failed to compile, display the info log and quit out
	if (shaderStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(m_id, infoLogLength, NULL, strInfoLog);

		// Output error
		OutputDebugString(strInfoLog);

		std::cout << m_type << " shader compilation failed: " << strInfoLog << std::endl;
		delete[] strInfoLog;
	}
	else
	{
		std::cout << m_type << " shader compilation OK" << std::endl;
	}
}

void TgShader::getError()
{
}