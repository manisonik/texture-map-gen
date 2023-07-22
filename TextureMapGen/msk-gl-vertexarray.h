#pragma once
#include "msk-core.h"

class MVertexArray : public MskVertexArray
{
private:
	std::vector<MskVertexAttribute> m_pVertexAttributes;
	int m_nStrideSize;

public:
	MVertexArray();
	virtual ~MVertexArray();

	void SetVertices(std::vector<GLfloat>& vertices, std::vector<MskVertexAttribute>& attributes);
};

