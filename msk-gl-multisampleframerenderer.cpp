#include "stdafx.h"
#include "msk-gl-multisampleframerenderer.h"

using namespace TexGen;

const char* vs_multisampleframebuffer = R"glsl(
	#version 440 core

	// Input vertex data, different for all executions of this shader.
	layout(location = 0) in vec3 vertexPosition_modelspace;
	layout(location = 1) in vec2 vertexUV;
	
	// Output uv
	out vec2 uv;

	void main()
	{
		gl_Position = vec4(vertexPosition_modelspace, 1.0);
		uv = vertexUV;
	}  
)glsl";

const char* fs_multisampleframebuffer = R"glsl(
	#version 440 core

	layout (binding = 0) uniform sampler2D tex;

	// Input color from vertex shader
	in vec2 uv;

	// Output
	out vec4 outColor;

	void main()
	{
		outColor = texture(tex, uv);
	} 
)glsl";


void TgMultisampleFrameRenderer::initTextures()
{
	// Setup texture data
	m_textureData.m_width = m_width;
	m_textureData.m_height = m_height;
	m_textureData.m_bytes = 0;

	m_texture.setFilter(eTextureFilter::Linear, eTextureFilter::Linear);
	m_texture.setFormat(eTextureFormat::RGBA);
	m_texture.setWrap(eTextureWrap::ClampToEdge, eTextureWrap::ClampToEdge);
	m_texture.setInternalFormat(eTextureFormat::RGBA);
	m_texture.loadFromMemory(m_textureData);
}

void TgMultisampleFrameRenderer::initFrameBuffer()
{
	glGetIntegerv(GL_MAX_SAMPLES, &m_maxSamples);

	// create a MSAA framebuffer object
	glGenFramebuffers(1, &m_fboMSAA);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboMSAA);

	// create a 4x MSAA renderbuffer object for colorbuffer
	glGenRenderbuffers(1, &m_fboColor);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboColor);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_RGB8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// create a MSAA renderbuffer object to store depth info
	// NOTE: A depth renderable image should be attached the FBO for depth test.
	// If we don't attach a depth renderable image to the FBO, then
	// the rendering output will be corrupted because of missing depth test.
	// If you also need stencil test for your rendering, then you must
	// attach additional image to the stencil attachement point, too.
	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach msaa RBOs to FBO attachment points
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_fboColor);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);

	// create a normal (no MSAA) FBO to hold a render-to-texture
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_rbo);

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach a texture to FBO color attachement point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.getTextureId(), 0);

	// attach a rbo to FBO depth attachement point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers); // "1" is the size of DrawBuffers*/

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
#ifdef _DEBUG
		OutputDebugString("Framebuffer Creation Failed.\n");
#endif
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TgMultisampleFrameRenderer::initShaders()
{
	TgShader vertexShader(eShaderType::Vertex);
	TgShader fragmentShader(eShaderType::Fragment);

	// Load and compile shaders
	vertexShader.loadFromString(vs_multisampleframebuffer);
	vertexShader.compile();
	fragmentShader.loadFromString(fs_multisampleframebuffer);
	fragmentShader.compile();

	// Attach shaders
	m_shaderProgram.attachShader(vertexShader);
	m_shaderProgram.attachShader(fragmentShader);
	m_shaderProgram.link();
}

void TgMultisampleFrameRenderer::initBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_tbo);
}

void TgMultisampleFrameRenderer::fillBuffers()
{
	m_vertexData = {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
	};

	m_texCoords = {
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
	};

	m_indexData = {
		2, 1, 0, 3, 1, 2
	};

	// Vertex Buffer
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(glm::vec3), &m_vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);  // Position

	// Texture Coords Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
	glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), &m_texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0); // Text coords

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size() * sizeof(GLuint), &m_indexData[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

TgMultisampleFrameRenderer::TgMultisampleFrameRenderer() : 
	m_width(1024),
	m_height(1024),
	m_samples(4)
	//m_vertexShader(eShaderType::Vertex),
	//m_fragmentShader(eShaderType::Fragment)
{
	initTextures();
	initFrameBuffer();
	initShaders();
	initBuffers();
	fillBuffers();
}

TgMultisampleFrameRenderer::~TgMultisampleFrameRenderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_fboColor);
	glDeleteBuffers(1, &m_fboDepth);
	glDeleteBuffers(1, &m_fboMSAA);
	glDeleteBuffers(1, &m_fbo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_tbo);
}

GLvoid TgMultisampleFrameRenderer::begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboMSAA);
	
	// clear buffer
	glClearColor(0.1, 0.1, 0.1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

GLvoid TgMultisampleFrameRenderer::end()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboMSAA);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	
	glViewport(0, 0, m_width, m_height);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_shaderProgram.use();

	// Bind texture
	glEnable(GL_TEXTURE_2D);
	m_texture.bind();

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_indexData.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_shaderProgram.disable();
}

void TgMultisampleFrameRenderer::resize(int width, int height)
{
	glBindTexture(m_texture.getTextureTarget(), m_texture.getTextureId());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(m_texture.getTextureTarget(), 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_fboColor);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_RGB8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// update internal dimensions
	m_width = width;
	m_height = height;
}

const TgTexture& TgMultisampleFrameRenderer::getTexture()
{
	return m_texture;
}
