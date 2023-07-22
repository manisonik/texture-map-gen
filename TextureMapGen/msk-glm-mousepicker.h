#pragma once
#include "msk-core.h"

class MMousePicker :
	public MskMousePicker
{
private:
	std::vector<std::shared_ptr<MskMesh>> m_vMeshes;
	std::vector<std::shared_ptr<MskRay>> m_vRay;
public:
	MMousePicker();
	~MMousePicker();

	GLvoid AddRay(std::shared_ptr<MskRay>& ray);
	GLvoid AddMesh(std::shared_ptr<MskMesh>& mesh);
	GLvoid Update();
};

