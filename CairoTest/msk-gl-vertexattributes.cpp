#include "stdafx.h"
#include "msk-gl-vertexattributes.h"


MVertexAttributes::MVertexAttributes()
{
	glGenVertexArrays(1, &m_hVAO);
}

MVertexAttributes::~MVertexAttributes()
{
	glDeleteBuffers(1, &m_hVAO);
}

void MVertexAttributes::Add(std::unique_ptr<MskVertexAttribute>& vertexAttribute)
{
	vertexAttribute->offset = m_nOffset;

	if (vertexAttribute->type == GL_FLOAT) {
		m_nOffset = vertexAttribute->componentCount * sizeof(float);
	}
	else if (vertexAttribute->type == GL_INT) {
		m_nOffset = vertexAttribute->componentCount * sizeof(int);
	}
	else if (vertexAttribute->type == GL_SHORT) {
		m_nOffset = vertexAttribute->componentCount * sizeof(short);
	}

	m_pVertexAttributes.push_back(std::move(vertexAttribute));
}

void MVertexAttributes::Begin()
{
	if (m_bBinded)
		throw std::invalid_argument("MVertexAttributes.End() must be called before Begin().");

	glBindVertexArray(m_hVAO);

	m_bBinded = true;
}

void MVertexAttributes::End()
{
	if (!m_bBinded)
		throw std::invalid_argument("MVertexAttributes.Begin() must be called before End().");

	m_bBinded = false;
}
