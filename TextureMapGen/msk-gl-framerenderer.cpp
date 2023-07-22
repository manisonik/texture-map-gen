#include "stdafx.h"
#include "msk-gl-framerenderer.h"

const char* vs_framebuffer = R"glsl(
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

const char* fs_framebuffer = R"glsl(
	#version 440 core

	// Input color from vertex shader
	in vec2 uv;

	// Output
	layout(location = 0) out vec4 outColor;

	// Texture
	uniform sampler2D renderedTexture;

	void main()
	{
		outColor = texture(renderedTexture, uv).rgba;
	} 
)glsl";


MFrameRenderer::MFrameRenderer(MskApp* mskApp)
{
	InitFrameBuffer(mskApp);
	InitShaders();
}

MFrameRenderer::~MFrameRenderer()
{
	glDeleteBuffers(1, &m_hFrameBuffer);
	m_pShaderProgram.reset();
	m_pTexture.reset();
}

void MFrameRenderer::InitFrameBuffer(MskApp* mskApp)
{
	// Create framebuffer
	glGenFramebuffers(1, &m_hFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFrameBuffer);

	// Enable texture
	glEnable(GL_TEXTURE_2D);

	// Generate Texture
	mskApp->CreateTexture(m_pTexture);
	m_pTexture->SetFilter(MskTextureFilter::Nearest, MskTextureFilter::Nearest);
	m_pTexture->SetFormat(MskTextureFormat::RGB);
	m_pTexture->SetInternalFormat(MskTextureFormat::RGB);

	std::shared_ptr<MskTextureData> data = std::shared_ptr<MskTextureData>(new MskTextureData());
	data->m_nWidth = 800;
	data->m_nHeight = 600;
	data->m_pBytes = 0;
	m_pTexture->LoadFromMemory(data);

	// The depth buffer
	glGenRenderbuffers(1, &m_hDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_hDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hDepthBuffer);

	// Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_pTexture->GetTextureTarget(),
		m_pTexture->GetTextureId(), 0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers); // "1" is the size of DrawBuffers

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		OutputDebugString("Framebuffer Creation Failed.\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MFrameRenderer::InitShaders() {
	// Create Vertex Shader
	std::shared_ptr<MskShader> vertexShader;
	std::shared_ptr<MskShader> fragmentShader;

	MskFactory::CreateShader(MskShaderType::Vertex, vertexShader);
	MskFactory::CreateShader(MskShaderType::Fragment, fragmentShader);
	MskFactory::CreateShaderProgram(m_pShaderProgram);

	vertexShader->LoadFromString(vs_framebuffer);
	vertexShader->Compile();
	fragmentShader->LoadFromString(fs_framebuffer);
	fragmentShader->Compile();
	m_pShaderProgram->AttachShader(vertexShader);
	m_pShaderProgram->AttachShader(fragmentShader);
	m_pShaderProgram->Link();
	m_pShaderProgram->AddUniform("renderedTexture");
}

GLvoid MFrameRenderer::Begin()
{
	m_pShaderProgram->Use();
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFrameBuffer);

	// Clear The Screen And The Depth Buffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLvoid MFrameRenderer::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_pShaderProgram->Disable();
}

std::shared_ptr<MskTexture> MFrameRenderer::GetTexture()
{
	return m_pTexture;
}
