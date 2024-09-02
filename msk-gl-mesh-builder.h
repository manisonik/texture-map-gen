#pragma once
#include "msk-gl-mesh.h"

namespace TexGen {
	class TgMeshBuilder
	{
	protected:
		dcel::manifold manifold;

	public:
		TgMeshBuilder();
		~TgMeshBuilder();

		void createCube();
		void createGrid();
		void createCylinder();
		void createSphere();
		void createIcoSphere();
		void createTest();

		const TgMesh& getMesh() const;

	private:
		TgMesh m_mesh;
		float m_nWidth;
		float m_nHeight;
		float m_nCount;
		uint32_t m_nStackCount;
		float m_nStackStep;
		float m_nStackAngle;
		uint32_t m_nSectorCount;
		float m_nSectorStep;
		float m_nSectorAngle;
		float m_nRadius;
	};
}

