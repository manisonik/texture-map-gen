#include "stdafx.h"
#include "msk-glm-mousepicker.h"

MMousePicker::MMousePicker()
{
}

MMousePicker::~MMousePicker()
{
}

GLvoid MMousePicker::AddRay(std::shared_ptr<MskRay>& ray)
{
    m_vRay.push_back(ray);
}

GLvoid MMousePicker::AddMesh(std::shared_ptr<MskMesh>& mesh)
{
    m_vMeshes.push_back(mesh);
}

GLvoid MMousePicker::Update()
{
	for (auto ray = m_vRay.begin(); ray != m_vRay.end(); ++ray) {
		for (auto mesh = m_vMeshes.begin(); mesh != m_vMeshes.end(); ++mesh)
			for (auto face = (*mesh)->GetFaces().begin(); face != (*mesh)->GetFaces().end(); ++face)
			if (face->indices.size() % 3 == 0) {
				glm::vec3 v0 = glm::vec3((*mesh)->GetModelMatrix() * glm::vec4((*mesh)->GetVertices()[face->indices[0]], 1.0f));
				glm::vec3 v1 = glm::vec3((*mesh)->GetModelMatrix() * glm::vec4((*mesh)->GetVertices()[face->indices[1]], 1.0f));
				glm::vec3 v2 = glm::vec3((*mesh)->GetModelMatrix() * glm::vec4((*mesh)->GetVertices()[face->indices[2]], 1.0f));

				glm::vec3 v3 = glm::vec3((*mesh)->GetModelMatrix() * glm::vec4((*mesh)->GetVertices()[face->indices[3]], 1.0f));
				glm::vec3 v4 = glm::vec3((*mesh)->GetModelMatrix() * glm::vec4((*mesh)->GetVertices()[face->indices[4]], 1.0f));
				glm::vec3 v5 = glm::vec3((*mesh)->GetModelMatrix() * glm::vec4((*mesh)->GetVertices()[face->indices[5]], 1.0f));

				glm::vec3 bary;
				if ((*ray)->Intersect(v0, v1, v2, bary) || (*ray)->Intersect(v3, v4, v5, bary))
				{

				}
			}
	}
}
