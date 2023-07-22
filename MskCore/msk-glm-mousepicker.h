#pragma once
#include "msk-glm-ray.h"
#include "msk-gl-mesh.h"

namespace Manisonik {
	class MMousePicker
	{
	private:
		std::vector<std::shared_ptr<MMesh>> m_vSelectedMeshes;
		std::vector<dcel::face_ptr> m_vSelectedFaces;
		std::vector<std::shared_ptr<MMesh>> m_vMeshes;
		std::shared_ptr<MRay> m_pRay;

	public:
		MMousePicker();
		~MMousePicker();

		std::vector<std::shared_ptr<MMesh>>& SelectedMesh();
		std::vector<dcel::face_ptr>& SelectedFaces();

		void AddMesh(std::shared_ptr<MMesh>& mesh);
		void Pick(std::shared_ptr<MRay>& ray);
	};
}

