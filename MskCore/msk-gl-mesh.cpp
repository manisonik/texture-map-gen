#include "stdafx.h"
#include "msk-gl-mesh.h"

using namespace Manisonik;

MMesh::MMesh()
{
	m_matModel = glm::mat4();
}

MMesh::~MMesh()
{
}

void MMesh::Rotate(GLfloat angle) {
	m_matRotate = glm::rotate(m_matIdentity, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
}

void MMesh::Translate(GLfloat dx, GLfloat dy, GLfloat dz) {
	m_matTranslate = glm::translate(m_matIdentity, glm::vec3(dx, dy, dz));
}

void MMesh::Scale(GLfloat x, GLfloat y, GLfloat z) {
	m_matScale = glm::scale(glm::vec3(x, y, z));
}

void MMesh::Update() {
	// Model Matrix
	m_matModel = m_matTranslate * m_matRotate * m_matScale;
}

GLvoid MMesh::Set(const std::shared_ptr<dcel::manifold>& manifold)
{
	m_pManifold = manifold;

	// Generate vertices
	m_vVertices = m_pManifold->generate_vertices();

	// Generate indices
	m_vIndices = m_pManifold->generate_face_indices();

	// Generate edge indices
	m_vEdgeIndices = m_pManifold->generate_edge_indices();
}

std::vector<uint32_t>& MMesh::GetEdgeIndices()
{
	return m_vEdgeIndices;
}

std::vector<uint32_t>& MMesh::GetIndices()
{
	return m_vIndices;
}

std::vector<dcel::vertex_attr>& MMesh::GetVertices()
{
	return m_vVertices;
}

std::shared_ptr<dcel::manifold>& MMesh::GetManifold()
{
	return m_pManifold;
}

glm::mat4 MMesh::GetModelMatrix()
{
	return m_matModel;
}
