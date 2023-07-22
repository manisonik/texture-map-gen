#include "stdafx.h"
#include "msk-gl-multisampleframerenderer.h"

using namespace Manisonik;

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


void MMultisampleFrameRenderer::InitTextures()
{
	// Setup texture data
	std::shared_ptr<MTextureData> data = std::make_shared<MTextureData>();
	data->m_nWidth = m_nWidth;
	data->m_nHeight = m_nHeight;
	data->m_pBytes = 0;

	m_pTexture = std::make_shared<MTexture>();
	m_pTexture->SetFilter(MskTextureFilter::Linear, MskTextureFilter::Linear);
	m_pTexture->SetFormat(MskTextureFormat::RGBA);
	m_pTexture->SetWrap(MskTextureWrap::ClampToEdge, MskTextureWrap::ClampToEdge);
	m_pTexture->SetInternalFormat(MskTextureFormat::RGBA);
	m_pTexture->LoadFromMemory(data);
}

void MMultisampleFrameRenderer::InitFrameBuffer()
{
	glGetIntegerv(GL_MAX_SAMPLES, &m_nMaxSamples);

	// create a MSAA framebuffer object
	glGenFramebuffers(1, &m_hFboMSAA);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFboMSAA);

	// create a 4x MSAA renderbuffer object for colorbuffer
	glGenRenderbuffers(1, &m_hFboColor);
	glBindRenderbuffer(GL_RENDERBUFFER, m_hFboColor);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_nSamples, GL_RGB8, m_nWidth, m_nHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// create a MSAA renderbuffer object to store depth info
	// NOTE: A depth renderable image should be attached the FBO for depth test.
	// If we don't attach a depth renderable image to the FBO, then
	// the rendering output will be corrupted because of missing depth test.
	// If you also need stencil test for your rendering, then you must
	// attach additional image to the stencil attachement point, too.
	glGenRenderbuffers(1, &m_hFboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_hFboDepth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_nSamples, GL_DEPTH24_STENCIL8, m_nWidth, m_nHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach msaa RBOs to FBO attachment points
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_hFboColor);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_hFboDepth);

	// create a normal (no MSAA) FBO to hold a render-to-texture
	glGenFramebuffers(1, &m_hFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFbo);

	glGenRenderbuffers(1, &m_hRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_hRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_nWidth, m_nHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach a texture to FBO color attachement point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->GetTextureId(), 0);

	// attach a rbo to FBO depth attachement point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_hRbo);

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

void MMultisampleFrameRenderer::InitShaders()
{
	// Create Vertex Shader
	std::shared_ptr<MShader> vertexShader = 
		std::make_shared<MShader>(MskShaderType::Vertex);

	// Create Fragment Shader
	std::shared_ptr<MShader> fragmentShader = 
		std::make_shared<MShader>(MskShaderType::Fragment);

	// Create shader program
	m_pShaderProgram = std::make_shared<MShaderProgram>();

	// Load and compile shaders
	vertexShader->LoadFromString(vs_multisampleframebuffer);
	vertexShader->Compile();
	fragmentShader->LoadFromString(fs_multisampleframebuffer);
	fragmentShader->Compile();

	// Attach shaders
	m_pShaderProgram->AttachShader(vertexShader);
	m_pShaderProgram->AttachShader(fragmentShader);
	m_pShaderProgram->Link();
}

void MMultisampleFrameRenderer::InitBuffers()
{
	glGenVertexArrays(1, &m_hVAO);
	glGenBuffers(1, &m_hVBO);
	glGenBuffers(1, &m_hIBO);
	glGenBuffers(1, &m_hTBO);
}

void MMultisampleFrameRenderer::FillBuffers()
{
	m_vVertexData = {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
	};

	m_vTexCoords = {
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
	};

	m_vIndexData = {
		2, 1, 0, 3, 1, 2
	};

	// Vertex Buffer
	glBindVertexArray(m_hVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertexData.size() * sizeof(glm::vec3), &m_vVertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);  // Position

	// Texture Coords Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hTBO);
	glBufferData(GL_ARRAY_BUFFER, m_vTexCoords.size() * sizeof(glm::vec2), &m_vTexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0); // Text coords

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndexData.size() * sizeof(GLuint), &m_vIndexData[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

MMultisampleFrameRenderer::MMultisampleFrameRenderer() : 
	m_nWidth(1024),
	m_nHeight(1024),
	m_nSamples(4)
{
	InitTextures();
	InitFrameBuffer();
	InitShaders();
	InitBuffers();
	FillBuffers();
}

MMultisampleFrameRenderer::~MMultisampleFrameRenderer()
{
	glDeleteVertexArrays(1, &m_hVAO);
	glDeleteBuffers(1, &m_hFboColor);
	glDeleteBuffers(1, &m_hFboDepth);
	glDeleteBuffers(1, &m_hFboMSAA);
	glDeleteBuffers(1, &m_hFbo);
	glDeleteBuffers(1, &m_hVBO);
	glDeleteBuffers(1, &m_hIBO);
	glDeleteBuffers(1, &m_hTBO);

	m_pShaderProgram.reset();
	m_pTexture.reset();
}

GLvoid MMultisampleFrameRenderer::Begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFboMSAA);
	
	// clear buffer
	glClearColor(0.1, 0.1, 0.1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

GLvoid MMultisampleFrameRenderer::End()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_hFboMSAA);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_hFbo);
	glBlitFramebuffer(0, 0, m_nWidth, m_nHeight, 0, 0, m_nWidth, m_nHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	
	glViewport(0, 0, m_nWidth, m_nHeight);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_pShaderProgram->Use();

	// Bind texture
	glEnable(GL_TEXTURE_2D);
	m_pTexture->Bind();

	glBindVertexArray(m_hVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glDrawElements(GL_TRIANGLES, m_vIndexData.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_pShaderProgram->Disable();
}

GLvoid MMultisampleFrameRenderer::Resize(GLsizei width, GLsizei height)
{
	glBindTexture(m_pTexture->GetTextureTarget(), m_pTexture->GetTextureId());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(m_pTexture->GetTextureTarget(), 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_hFboColor);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_nSamples, GL_RGB8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_hFboDepth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_nSamples, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_hRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// update internal dimensions
	m_nWidth = width;
	m_nHeight = height;
}

std::shared_ptr<MTexture> MMultisampleFrameRenderer::GetTexture()
{
	return m_pTexture;
}
