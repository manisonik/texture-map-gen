#include "stdafx.h"
#include "msk-gl-vertexarray.h"


MVertexArray::MVertexArray()
{
}

MVertexArray::~MVertexArray()
{
}

void MVertexArray::SetVertices(std::vector<GLfloat>& vertices, std::vector<MskVertexAttribute>& attributes)
{
	for (auto i = attributes.begin(); i != attributes.end(); i++) {
		switch ((*i).usage) {
		case MskVertexUsage::Position: 
		{
		}
		break;
		case MskVertexUsage::Color:
			break;
		case MskVertexUsage::TexCoords:
			break;
		case MskVertexUsage::Normal:
			break;
		case MskVertexUsage::Tangent:
			break;
		case MskVertexUsage::BiNormal:
			break;
		}
	}
}
