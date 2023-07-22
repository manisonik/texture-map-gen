#pragma once
#include "msk-gl-mesh.h"

namespace Manisonik {
	class MMeshBuilder
	{
	protected:
		std::shared_ptr<dcel::manifold> manifold;

	public:
		MMeshBuilder();
		~MMeshBuilder();

		GLvoid CreateCube(std::shared_ptr<MMesh>& mesh);
		GLvoid CreateGrid(std::shared_ptr<MMesh>& mesh);
		GLvoid CreateCylinder(std::shared_ptr<MMesh>& mesh);
		GLvoid CreateSphere(std::shared_ptr<MMesh>& mesh);
		GLvoid CreateIcoSphere(std::shared_ptr<MMesh>& mesh);
		GLvoid CreateTest(std::shared_ptr<MMesh>& mesh);

	private:
		GLfloat m_nWidth;
		GLfloat m_nHeight;
		GLfloat m_nCount;
		GLuint m_nStackCount;
		GLfloat m_nStackStep;
		GLfloat m_nStackAngle;
		GLuint m_nSectorCount;
		GLfloat m_nSectorStep;
		GLfloat m_nSectorAngle;
		GLfloat m_nRadius;
	};
}

