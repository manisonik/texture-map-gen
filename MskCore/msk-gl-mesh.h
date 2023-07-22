#pragma once

namespace Manisonik {
	class MMesh
	{
	private:
		std::shared_ptr<dcel::manifold> m_pManifold;
		std::vector<uint32_t> m_vIndices;
		std::vector<uint32_t> m_vEdgeIndices;
		std::vector<dcel::vertex_attr> m_vVertices;
		glm::mat4 m_matModel;
		glm::mat4 m_matIdentity;
		glm::mat4 m_matRotate;
		glm::mat4 m_matTranslate;
		glm::mat4 m_matScale;

	public:
		MMesh();
		~MMesh();

		friend class MRenderer;

		void Update();
		void Rotate(GLfloat angle);
		void Translate(GLfloat dx, GLfloat dy, GLfloat dz);
		void Scale(GLfloat x, GLfloat y, GLfloat z);
		void Set(const std::shared_ptr<dcel::manifold>& manifold);
		std::vector<uint32_t>& GetEdgeIndices();
		std::vector<uint32_t>& GetIndices();
		std::vector<dcel::vertex_attr>& GetVertices();
		std::shared_ptr<dcel::manifold>& GetManifold();
		glm::mat4 GetModelMatrix();
	};
}

