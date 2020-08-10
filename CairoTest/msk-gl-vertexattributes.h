#pragma once
#include "msk-core.h"

class MVertexAttributes : public MskVertexAttributes
{
private:
	GLuint m_nOffset;
	GLboolean m_bBinded;
	GLuint m_hVAO;
	std::vector<std::unique_ptr<MskVertexAttribute>> m_pVertexAttributes;

public:
	MVertexAttributes();
	virtual ~MVertexAttributes();

	void Add(std::unique_ptr<MskVertexAttribute>& vertexAttribute);
	void Begin();
	void End();
};

