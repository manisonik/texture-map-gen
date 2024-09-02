#include "stdafx.h"
#include "msk-glm-mousepicker.h"

using namespace TexGen;

TgMousePicker::TgMousePicker() :
	m_ray(glm::vec3(0,0,0))
{
}

TgMousePicker::~TgMousePicker()
{

}

const std::vector<std::shared_ptr<TgMesh>>& TgMousePicker::selectedMesh() const
{
	return m_selectedMeshes;
}

const std::vector<dcel::face_ptr>& TgMousePicker::selectedFaces() const
{
	return m_selectedFaces;
}

void TgMousePicker::addMesh(const std::shared_ptr<TgMesh>& mesh)
{
	m_meshes.push_back(mesh);
}

void TgMousePicker::pick(const TgRay& ray)
{
	m_selectedFaces.clear();

	// Check against faces
	for (auto mesh : m_meshes) {
		auto m = mesh->getManifold();
		for (dcel::face_ptr f : m.faces()) {
			f->triangulate([this, f, ray](glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
				glm::vec3 bary;
				if (ray.intersect(v0, v1, v2, bary)) {
					m_selectedFaces.push_back(f);
				}
				});
		}
	}
}