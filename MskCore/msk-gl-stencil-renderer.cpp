#include "stdafx.h"
#include "msk-gl-stencil-renderer.h"

using namespace Manisonik;

MStencilRenderer::MStencilRenderer()
{
	m_bDrawing = false;
}

MStencilRenderer::~MStencilRenderer()
{
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

void MStencilRenderer::Draw(const std::function<void(int)>& renderPass)
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