#pragma once
#include "msk-glm-ray.h"
#include "msk-gl-mesh.h"

namespace TexGen {
	class TgMousePicker
	{
	private:
		std::vector<std::shared_ptr<TgMesh>> m_selectedMeshes;
		std::vector<dcel::face_ptr> m_selectedFaces;
		std::vector<std::shared_ptr<TgMesh>> m_meshes;
		TgRay m_ray;

	public:
		TgMousePicker();
		~TgMousePicker();

		const std::vector<std::shared_ptr<TgMesh>>& selectedMesh() const;
		const std::vector<dcel::face_ptr>& selectedFaces() const;

		void addMesh(const std::shared_ptr<TgMesh>& mesh);
		void pick(const TgRay& ray);
	};
}

