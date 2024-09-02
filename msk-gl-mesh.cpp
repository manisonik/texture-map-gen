#include "stdafx.h"
#include "msk-gl-mesh.h"

using namespace TexGen;

TgMesh::TgMesh() :
	m_matModel(glm::mat4(1.0)),
	m_matScale(glm::mat4(1.0)),
	m_matTranslate(glm::mat4(1.0)),
	m_matRotate(glm::mat4(1.0))
{
}

TgMesh::~TgMesh()
{
}

void TgMesh::rotate(float angle) {
	m_matRotate = glm::rotate(m_matIdentity, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
}

void TgMesh::translate(float dx, float dy, float dz) {
	m_matTranslate = glm::translate(m_matIdentity, glm::vec3(dx, dy, dz));
}

void TgMesh::scale(float x, float y, float z) {
	m_matScale = glm::scale(glm::vec3(x, y, z));
}

void TgMesh::update() {
	// Model Matrix
	m_matModel = m_matTranslate * m_matRotate * m_matScale;
}

void TgMesh::set(const dcel::manifold& manifold)
{
	m_manifold = manifold;

	// Generate vertices
	m_vertices = m_manifold.generate_vertices();

	// Generate indices
	m_indices = m_manifold.generate_face_indices();

	// Generate edge indices
	m_edgeIndices = m_manifold.generate_edge_indices();
}

const std::vector<uint32_t>& TgMesh::getEdgeIndices() const
{
	return m_edgeIndices;
}

const std::vector<uint32_t>& TgMesh::getIndices() const
{
	return m_indices;
}

const std::vector<dcel::vertex_attr>& TgMesh::getVertices() const
{
	return m_vertices;
}

const dcel::manifold& TgMesh::getManifold() const
{
	return m_manifold;
}

const glm::mat4& TgMesh::getModelMatrix() const
{
	return m_matModel;
}
