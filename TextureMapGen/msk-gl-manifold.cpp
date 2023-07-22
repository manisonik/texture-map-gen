#include "stdafx.h"
#include "msk-gl-manifold.h"

MManifold::MManifold()
{
}

MManifold::~MManifold()
{
}

// Appends to the last edge
std::shared_ptr<MskFace2> MManifold::AddFace(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
	// Create face
	std::shared_ptr<MskFace2> face = std::make_shared<MskFace2>();

	// Create edges
	std::shared_ptr<MskEdge> edge0 = std::make_shared<MskEdge>();
	std::shared_ptr<MskEdge> edge1 = std::make_shared<MskEdge>();
	std::shared_ptr<MskEdge> edge2 = std::make_shared<MskEdge>();

	// Save the first edge
	if (!firstEdge.use_count()) {
		firstEdge = edge0;
	}

	// Set the face normal
	face->faceNormal = glm::triangleNormal(v0, v1, v2);

	// Set vertices
	std::shared_ptr<MskVertex> vert0 = std::make_shared<MskVertex>();
	vert0->edge = edge0;
	vert0->v0 = v0;
	vert0->v1 = v1;
	vert0->v2 = v2;

	std::shared_ptr<MskVertex> vert1 = std::make_shared<MskVertex>();
	vert1->edge = edge1;
	vert1->v0 = v0;
	vert1->v1 = v1;
	vert1->v2 = v2;

	std::shared_ptr<MskVertex> vert2 = std::make_shared<MskVertex>();
	vert2->edge = edge2;
	vert2->v0 = v0;
	vert2->v1 = v1;
	vert2->v2 = v2;

	// Set the tail vertex edge 0
	edge0->vert = vert1;
	edge0->next = edge1;
	edge0->face = face;

	// Set the tail vertex edge 1
	edge1->vert = vert2;
	edge1->next = edge2;
	edge1->face = face;

	// Set the tail vertex edge 2
	edge2->vert = vert0;
	edge2->next = edge0;
	edge2->face = face;

	// Save the last edge
	lastEdge = edge2;

	return face;
}
