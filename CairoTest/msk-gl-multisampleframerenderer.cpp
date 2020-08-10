#include "stdafx.h"
#include "msk-gl-multisampleframerenderer.h"

const char* vs_multisampleframebuffer = R"glsl(
	#version 440 core

	// Input vertex data, different for all executions of this shader.
	in vec2 vertexPos;
	in vec2 vertexUV;
	out vec2 uv;

	void main()
	{
		gl_Position = vec4(vertexPos, 0.0, 1.0); 
		uv = vertexUV;
	}  
)glsl";

const char* fs_multisampleframebuffer = R"glsl(
	#version 440 core

	// Input color from vertex shader
	in vec2 uv;

	// Output
	layout(location = 0) out vec4 outColor;

	// Texture
	uniform sampler2DMS renderedTexture;

	void main()
	{
		outColor = texelFetch(renderedTexture, uv, 3);
	} 
)glsl";


void MMultisampleFrameRenderer::InitTextures(MskApp* mskApp)
{
	// Setup texture data
	std::shared_ptr<MskTextureData> data = std::shared_ptr<MskTextureData>(new MskTextureData());
	data->m_nWidth = m_nWidth;
	data->m_nHeight = m_nHeight;
	data->m_pBytes = 0;

	// Generate Texture
	mskApp->CreateTexture(m_pTexture);
	m_pTexture->SetFilter(MskTextureFilter::Linear, MskTextureFilter::Linear);
	m_pTexture->SetFormat(MskTextureFormat::RGBA);
	m_pTexture->SetWrap(MskTextureWrap::ClampToEdge, MskTextureWrap::ClampToEdge);
	m_pTexture->SetInternalFormat(MskTextureFormat::RGBA);
	//m_pTexture->EnableMultisampling(4);
	m_pTexture->LoadFromMemory(data);
}

void MMultisampleFrameRenderer::InitFrameBuffer(MskApp* mskApp)
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
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_nSamples, GL_DEPTH_COMPONENT, m_nWidth, m_nHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach msaa RBOs to FBO attachment points
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_hFboColor);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hFboDepth);

	// create a normal (no MSAA) FBO to hold a render-to-texture
	glGenFramebuffers(1, &m_hFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFbo);

	glGenRenderbuffers(1, &m_hRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_hRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_nWidth, m_nHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach a texture to FBO color attachement point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->GetTextureId(), 0);

	// attach a rbo to FBO depth attachement point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hRbo);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers); // "1" is the size of DrawBuffers*/

	// The depth buffer
	/*glGenRenderbuffers(1, &m_hDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_hDepthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hDepthBuffer);

	// Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_pTexture->GetTextureTarget(),
		m_pTexture->GetTextureId(), 0);

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
	/*std::shared_ptr<MskShader> vertexShader;
	std::shared_ptr<MskShader> fragmentShader;

	MskFactory::CreateShader(MskShaderType::Vertex, vertexShader);
	MskFactory::CreateShader(MskShaderType::Fragment, fragmentShader);
	MskFactory::CreateShaderProgram(m_pShaderProgram);

	vertexShader->LoadFromString(vs_multisampleframebuffer);
	vertexShader->Compile();
	fragmentShader->LoadFromString(fs_multisampleframebuffer);
	fragmentShader->Compile();
	m_pShaderProgram->AttachShader(vertexShader);
	m_pShaderProgram->AttachShader(fragmentShader);
	m_pShaderProgram->Link();
	m_pShaderProgram->AddUniform("renderedTexture");*/
}

MMultisampleFrameRenderer::MMultisampleFrameRenderer(MskApp* mskApp) : 
	m_nWidth(1024),
	m_nHeight(1024),
	m_nSamples(4)
{
	InitTextures(mskApp);
	InitFrameBuffer(mskApp);
	InitShaders();
}

MMultisampleFrameRenderer::~MMultisampleFrameRenderer()
{
	glDeleteBuffers(1, &m_hFboColor);
	glDeleteBuffers(1, &m_hFboDepth);
	glDeleteBuffers(1, &m_hFboMSAA);
	glDeleteBuffers(1, &m_hFbo);
	m_pShaderProgram.reset();
	m_pTexture.reset();
}

GLvoid MMultisampleFrameRenderer::Begin()
{
	//m_pShaderProgram->Use();
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFboMSAA);
	
	// clear buffer
	glClearColor(0.1, 0.1, 0.1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLvoid MMultisampleFrameRenderer::End()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_hFboMSAA);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_hFbo);
	glBlitFramebuffer(0, 0, m_nWidth, m_nHeight, 0, 0, m_nWidth, m_nHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//m_pShaderProgram->Disable();
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
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_nSamples, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_hRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// update internal dimensions
	m_nWidth = width;
	m_nHeight = height;
}

std::shared_ptr<MskTexture> MMultisampleFrameRenderer::GetTexture()
{
	return m_pTexture;
}
