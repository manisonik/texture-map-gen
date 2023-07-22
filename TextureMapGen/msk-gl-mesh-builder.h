#pragma once
#include "msk-core.h"

class MMskMeshBuilder : MskMeshBuilder
{
public:
		MMskMeshBuilder(MskApp* mskApp);
		~MMskMeshBuilder();

		GLvoid CreateCyclinder(std::shared_ptr<MskMesh>& mesh);
		GLvoid CreateCube(std::shared_ptr<MskMesh>& mesh);
		GLvoid CreateCone(std::shared_ptr<MskMesh>& mesh);
		GLvoid CreatePlane(std::shared_ptr<MskMesh>& mesh);
		GLvoid CreateIcoSpere(std::shared_ptr<MskMesh>& mesh);
		GLvoid CreateTorus(std::shared_ptr<MskMesh>& mesh);
		GLvoid CreateCircle(std::shared_ptr<MskMesh>& mesh);
		GLvoid CreateUVSphere(std::shared_ptr<MskMesh>& mesh);
		GLvoid CreateGrid(std::shared_ptr<MskMesh>& mesh);
};