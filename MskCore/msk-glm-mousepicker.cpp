#include "stdafx.h"
#include "msk-glm-mousepicker.h"

using namespace Manisonik;

MMousePicker::MMousePicker()
{
}

MMousePicker::~MMousePicker()
{

}

std::vector<std::shared_ptr<MMesh>>& MMousePicker::SelectedMesh()
{
	return m_vSelectedMeshes;
}

std::vector<dcel::face_ptr>& MMousePicker::SelectedFaces()
{
	return m_vSelectedFaces;
}

void MMousePicker::AddMesh(std::shared_ptr<MMesh>& mesh)
{
	m_vMeshes.push_back(mesh);
}

void MMousePicker::Pick(std::shared_ptr<MRay>& ray)
{
	m_vSelectedFaces.clear();

	// Check against faces
	for (auto mesh : m_vMeshes) {
		std::shared_ptr<dcel::manifold> m = mesh->GetManifold();
		for (dcel::face_ptr f : m->faces()) {
			f->triangulate([this, f, ray](glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
				glm::vec3 bary;
				if (ray->Intersect(v0, v1, v2, bary)) {
					m_vSelectedFaces.push_back(f);
				}
				});
		}
	}
}