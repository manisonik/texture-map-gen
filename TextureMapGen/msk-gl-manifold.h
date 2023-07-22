#pragma once
#include "msk-core.h"

class MManifold : MskManifold
{
public:
	MManifold();
	~MManifold();

	std::shared_ptr<MskFace2> AddFace(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
private:
	std::shared_ptr<MskFace2> faces;
	std::shared_ptr<MskVertex> verts;
	std::shared_ptr<MskEdge> edges;
	std::shared_ptr<MskEdge> lastEdge;
	std::shared_ptr<MskEdge> firstEdge;
};

