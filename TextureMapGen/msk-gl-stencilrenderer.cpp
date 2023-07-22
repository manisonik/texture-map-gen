#include "stdafx.h"
#include "msk-gl-stencilrenderer.h"

MStencilRenderer::MStencilRenderer(MskApp* mskApp) :
	m_bDrawing(false)
{
	m_pMskApp = mskApp;
}

MStencilRenderer::~MStencilRenderer()
{
}

GLvoid MStencilRenderer::InitShaders()
{
	m_pMskApp->CreateShader(MskShaderType::Vertex, m_pVertexShader);
	m_pMskApp->CreateShader(MskShaderType::Fragment, m_pFragmentShader);
	m_pMskApp->CreateShaderProgram(m_pShaderProgram);

	m_pVertexShader->LoadFromFile(".\\basic_texture.glvs");
	m_pVertexShader->Compile();
	m_pFragmentShader->LoadFromFile(".\\basic_texture.glfs");
	m_pFragmentShader->Compile();

	m_pShaderProgram->AttachShader(m_pVertexShader);
	m_pShaderProgram->AttachShader(m_pFragmentShader);
	m_pShaderProgram->Link();

	// Add MVP matrix
	m_pShaderProgram->AddUniform("MVP");
}


void MStencilRenderer::Begin()
{
	if (m_bDrawing)
		throw std::invalid_argument("MStencilRenderer.End() must be called before Begin().");
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0x00);

	m_bDrawing = true;
}

void MStencilRenderer::Draw(std::function<void(int)> renderPass) 
{
	// Set the stencil buffer to write a 1 in every time
	// a pixel is written to the screen
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	renderPass(0);

	// Set the stencil buffer to only allow writing
	// to the screen when the value of the
	// stencil buffer is not 1
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	
	renderPass(1);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
}

void MStencilRenderer::End()
{
	if (!m_bDrawing)
		throw std::invalid_argument("MStencilRenderer.Begin() must be called before End().");

	glDisable(GL_STENCIL_TEST);

	m_bDrawing = false;
}

void MStencilRenderer::Flush()
{
}

void MStencilRenderer::Draw(std::shared_ptr<MskMesh>& mesh)
{
}

void MStencilRenderer::SetProjectionMatrix(glm::mat4& matProj)
{
}
