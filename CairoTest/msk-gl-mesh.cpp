#include "stdafx.h"
#include "msk-gl-mesh.h"

MMesh::MMesh()
{
	m_nTotalVertexComponents = 0;
	m_matScale = glm::mat4();
	m_matTranslate = glm::mat4();
	m_matRotate = glm::mat4();
	m_matModel = glm::mat4();
	m_matIdentity = glm::mat4();
	m_vColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

MMesh::~MMesh() {}

GLvoid MMesh::SetColor(glm::vec4 color)
{
	m_vColor = color;
}

GLvoid MMesh::AddPlane() {
	std::vector<glm::vec3> vertices = {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
	};

	std::vector<glm::vec2> texCoords = {
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
	};

	std::vector<GLshort> indices = {
		0, 1, 2, 2, 1, 3
	};

	SetVertices(vertices);
	SetIndices(indices);
	SetTextureCoords(texCoords);

	m_pFaces.push_back({ std::vector<GLshort> {0, 1, 2, 2, 1, 3} });

	CalculateNormals();
	CalculateTangentBasis();

	m_pSubset.push_back({
		m_nBaseVertex,
		vertices.size()
		});

	m_nBaseVertex += vertices.size();
}

GLvoid MMesh::CalculateNormals()
{
	for (size_t i = 0; i < m_pIndices.size(); i += 3)
	{ 
		glm::vec3 normal = glm::triangleNormal(
			glm::vec3(m_pVertices[m_pIndices[i]]),
			glm::vec3(m_pVertices[m_pIndices[i + 1]]),
			glm::vec3(m_pVertices[m_pIndices[i + 2]])
		);

		m_pNormals.insert(m_pNormals.end(), 3, normal);
	}
}

GLvoid MMesh::CalculateTangentBasis()
{
	if (m_pIndices.size() < 3)
		return;

	for (size_t i = 0; i < m_pIndices.size(); i += 3) {
		glm::vec3& v0 = glm::vec3(m_pVertices[m_pIndices[i]]);
		glm::vec3& v1 = glm::vec3(m_pVertices[m_pIndices[i + 1]]);
		glm::vec3& v2 = glm::vec3(m_pVertices[m_pIndices[i + 2]]);

		glm::vec2& uv0 = m_pTexCoords[m_pIndices[i]];
		glm::vec2& uv1 = m_pTexCoords[m_pIndices[i + 1]];
		glm::vec2& uv2 = m_pTexCoords[m_pIndices[i + 2]];

		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		m_pTangents.insert(m_pTangents.end(), 3, tangent);
		m_pBitangents.insert(m_pBitangents.end(), 3, bitangent);
	}
}

void MMesh::AddGrid(GLfloat width, GLfloat height, GLfloat count) {
	float cw = (float)width / count;
	float ch = (float)width / count;
	float hh = (float)height / 2;
	float hw = (float)width / 2;

	std::vector<GLshort> indices;
	std::vector<glm::vec3> vertices;

	vertices = {
		  glm::vec3(0.0f, 0.0f,  hh),  
		  glm::vec3(0.0f, 0.0f, -hh),  
		  glm::vec3(hw,   0.0f,  0.0f),
		  glm::vec3(-hw,   0.0f,  0.0f),
	};

	for (int i = 1; i <= count / 2; i++) {
		vertices.insert(vertices.end(), {
			 glm::vec3(i * cw, 0.0f,  hh), 
			 glm::vec3(i * cw, 0.0f, -hh),
			 glm::vec3(-i * cw, 0.0f,  hh), 
			 glm::vec3(-i * cw, 0.0f, -hh),
			});
	}

	for (int i = 1; i <= count / 2; i++) {
		vertices.insert(vertices.end(), {
			glm::vec3(hw, 0.0f,  i * ch), 
			glm::vec3(-hw, 0.0f, i * ch),
			glm::vec3(-hw, 0.0f,  -i * ch), 
			glm::vec3(hw, 0.0f, -i * ch),
			});
	}

	for (GLshort i = 0; i < vertices.size(); i++) {
		m_pFaces.push_back({ std::vector<GLshort> {
			(GLshort)(i * 2), 
			(GLshort)(i * 2 + 1)
		}});
	}

	SetVertices(vertices);
	SetIndices(indices);
}

GLvoid MMesh::AddCube()
{
	std::vector<glm::vec3> vertices = {
		// Back
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),

		// Front
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),

		// Left
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),

		// Right
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),

		// Bottom
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),

		// Top
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
	};

	std::vector<glm::vec3> normals =
	{
		 // Back
		 glm::vec3(0.0f, 0.0f, -1.0f),
		 glm::vec3(0.0f, 0.0f, -1.0f),
		 glm::vec3(0.0f, 0.0f, -1.0f),
		 glm::vec3(0.0f, 0.0f, -1.0f),

		 // Front
		 glm::vec3(0.0f, 0.0f, 1.0f),
		 glm::vec3(0.0f, 0.0f, 1.0f),
		 glm::vec3(0.0f, 0.0f, 1.0f),
		 glm::vec3(0.0f, 0.0f, 1.0f),

		 // Left
		 glm::vec3(-1.0f, 0.0f, 0.0f),
		 glm::vec3(-1.0f, 0.0f, 0.0f),
		 glm::vec3(-1.0f, 0.0f, 0.0f),
		 glm::vec3(-1.0f, 0.0f, 0.0f),

		 // Right
		 glm::vec3(1.0f, 0.0f, 0.0f),
		 glm::vec3(1.0f, 0.0f, 0.0f),
		 glm::vec3(1.0f, 0.0f, 0.0f),
		 glm::vec3(1.0f, 0.0f, 0.0f),

		 // Bottom
		 glm::vec3(0.0f, -1.0f, 0.0f),
		 glm::vec3(0.0f, -1.0f, 0.0f),
		 glm::vec3(0.0f, -1.0f, 0.0f),
		 glm::vec3(0.0f, -1.0f, 0.0f),

		 // Top
		 glm::vec3(0.0f, 1.0f, 0.0f),
		 glm::vec3(0.0f, 1.0f, 0.0f),
		 glm::vec3(0.0f, 1.0f, 0.0f),
		 glm::vec3(0.0f, 1.0f, 0.0f),
	};

	std::vector<glm::vec2> texCoords = {
		// Back
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Front
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Left
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Right
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Bottom
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Top
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
	};

	std::vector<GLshort> indices = {
		0, 1, 2, 2, 1, 3, // Back
		4, 5, 6, 6, 5, 7, // Front
		8, 9, 10, 10, 9, 11, // Left
		12, 13, 14, 14, 13, 15, // Right
		16, 17, 18, 18, 17, 19, // Bottom
		20, 21, 22, 22, 21, 23  // Top
	};

	m_pFaces.push_back({ std::vector<GLshort> {0, 1, 2, 2, 1, 3} });
	m_pFaces.push_back({ std::vector<GLshort> {4, 5, 6, 6, 5, 7} });
	m_pFaces.push_back({ std::vector<GLshort> {8, 9, 10, 10, 9, 11} });
	m_pFaces.push_back({ std::vector<GLshort> {12, 13, 14, 14, 13, 15} });
	m_pFaces.push_back({ std::vector<GLshort> {16, 17, 18, 18, 17, 19} });
	m_pFaces.push_back({ std::vector<GLshort> {20, 21, 22, 22, 21, 23} });

	SetVertices(vertices);
	SetIndices(indices);
	SetTextureCoords(texCoords);
	SetNormals(normals);
	CalculateTangentBasis();

	m_nBaseVertex += vertices.size();
}

GLvoid MMesh::SetVertices(std::vector<glm::vec3>& vertices) { m_pVertices = vertices; }

GLvoid MMesh::SetNormals(std::vector<glm::vec3>& normals) { m_pNormals = normals; }

GLvoid MMesh::SetIndices(std::vector<GLshort>& indices) { m_pIndices = indices; }

GLvoid MMesh::SetTextureCoords(std::vector<glm::vec2>& texCoords) { m_pTexCoords = texCoords; }

GLvoid MMesh::SetVertexAttributes(std::vector<MskVertexAttribute>& vertexAttributes)
{
	m_pVertexAttributes = vertexAttributes;
}

GLvoid MMesh::Rotate(GLfloat angle) {
	m_matRotate = glm::rotate(m_matIdentity, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
}

GLvoid MMesh::Translate(GLfloat dx, GLfloat dy, GLfloat dz) {
	m_matTranslate = glm::translate(m_matIdentity, glm::vec3(dx, dy, dz));
}

GLvoid MMesh::Translate(GLuint subset, GLfloat dx, GLfloat dy, GLfloat dz) {
	glm::mat4 translate = glm::translate(m_matIdentity, glm::vec3(dx, dy, dz));
}

GLvoid MMesh::Scale(GLfloat x, GLfloat y, GLfloat z) {
	m_matScale = glm::scale(glm::vec3(x, y, z));
}

glm::mat4& MMesh::GetModelMatrix() { return m_matModel; }

std::vector<MskFace>& MMesh::GetFaces()
{
	return m_pFaces;
}

std::vector<glm::vec3>& MMesh::GetNormals()
{
	return m_pNormals;
}

std::vector<glm::vec3>& MMesh::GetTangents()
{
	return m_pTangents;
}

std::vector<glm::vec3>& MMesh::GetBitangents()
{
	return m_pBitangents;
}

GLvoid MMesh::Update() {
	// Model Matrix
	m_matModel = m_matTranslate * m_matRotate * m_matScale;
}

std::vector<glm::vec3>& MMesh::GetVertices() {
	return m_pVertices;
}

std::vector<GLshort>& MMesh::GetIndices() {
	return m_pIndices;
}

std::vector<glm::vec2>& MMesh::GetTextureCoords()
{
	return m_pTexCoords;
}

glm::vec4& MMesh::GetColor()
{
	return m_vColor;
}
