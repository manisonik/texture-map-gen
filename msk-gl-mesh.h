#pragma once

namespace TexGen {
	class TgMesh
	{
	private:
		dcel::manifold m_manifold;
		std::vector<unsigned int> m_indices;
		std::vector<unsigned int> m_edgeIndices;
		std::vector<dcel::vertex_attr> m_vertices;
		glm::mat4 m_matModel;
		glm::mat4 m_matIdentity;
		glm::mat4 m_matRotate;
		glm::mat4 m_matTranslate;
		glm::mat4 m_matScale;

	public:
		TgMesh();
		~TgMesh();

		friend class MRenderer;

		void update();
		void rotate(float angle);
		void translate(float dx, float dy, float dz);
		void scale(float x, float y, float z);
		void set(const dcel::manifold& manifold);
		const std::vector<unsigned int>& getEdgeIndices() const;
		const std::vector<unsigned int>& getIndices() const;
		const std::vector<dcel::vertex_attr>& getVertices() const;
		const dcel::manifold& getManifold() const;
		const glm::mat4& getModelMatrix() const;
	};
}

