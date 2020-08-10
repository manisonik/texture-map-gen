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
		glm::vec3(-1.0f, -1.0f,  0.0f), // 0
		glm::vec3(-1.0f,  1.0f,  0.0f), // 1
		glm::vec3(1.0f, -1.0f,  0.0f), // 2
		glm::vec3(1.0f,  1.0f,  0.0f), // 3
	};

	std::vector<glm::vec2> texCoords = {
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(0.0f, 1.0f), // 1
		glm::vec2(1.0f, 0.0f), // 2
		glm::vec2(1.0f, 1.0f)  // 3
	};

	std::vector<GLshort> indices = {
		2, 1, 0, 3, 1, 2
	};

	SetVertices(vertices);
	SetIndices(indices);
	SetTextureCoords(texCoords);

	CalculateNormals();

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

void MMesh::AddGrid(GLfloat width, GLfloat height, GLfloat count) {
	float cw = (float)width / count;
	float ch = (float)width / count;
	float hh = (float)height / 2;
	float hw = (float)width / 2;

	std::vector<GLshort> indices;
	std::vector<glm::vec3> vertices;

	vertices = {
		  glm::vec3(0.0f, 0.0f,  hh),   //1.0f, 1.0f, 0.0f,  0.0f, 0.3f,
		  glm::vec3(0.0f, 0.0f, -hh),   //1.0f, 1.0f, 0.0f,  0.0f, 0.3f,
		  glm::vec3(hw,   0.0f,  0.0f), //1.0f, 0.0f, 0.98f, 0.3f, 0.3f,
		  glm::vec3(-hw,   0.0f,  0.0f), //1.0f, 0.0f, 0.98f, 0.3f, 0.3f
	};

	for (int i = 1; i <= count / 2; i++) {
		vertices.insert(vertices.end(), {
			 glm::vec3(i * cw, 0.0f,  hh), //1.0f, 1.0f, 1.0f, 1.0f, 0.1f,
			 glm::vec3(i * cw, 0.0f, -hh), //1.0f, 1.0f, 1.0f, 1.0f, 0.1f,
			 glm::vec3(-i * cw, 0.0f,  hh), //1.0f, 1.0f, 1.0f, 1.0f, 0.1f,
			 glm::vec3(-i * cw, 0.0f, -hh), //1.0f, 1.0f, 1.0f, 1.0f, 0.1f
			});
	}

	for (int i = 1; i <= count / 2; i++) {
		vertices.insert(vertices.end(), {
			glm::vec3(hw, 0.0f,  i * ch), //1.0f, 1.0f, 1.0f, 1.0f,  0.1f,
			glm::vec3(-hw, 0.0f,  i * ch), //1.0f, 1.0f, 1.0f, 1.0f,  0.1f,
			glm::vec3(hw, 0.0f, -i * ch), //1.0f, 1.0f, 1.0f, 1.0f,  0.1f,
			glm::vec3(-hw, 0.0f, -i * ch), //1.0f, 1.0f, 1.0f, 1.0f,  0.1f
			});
	}

	for (size_t i = 0; i < vertices.size(); i++) {
		indices.push_back(i * 2);
		indices.push_back(i * 2 + 1);
	}

	SetVertices(vertices);
	SetIndices(indices);

	m_nBaseVertex += vertices.size();
}

GLvoid MMesh::AddCube()
{
	/*std::vector<glm::vec3> vertices = {
	glm::vec3(-0.5f, -0.5f,  0.5f),
	glm::vec3(-0.5f,  0.5f,  0.5f),
	glm::vec3( 0.5f, -0.5f,  0.5f), 
	glm::vec3( 0.5f,  0.5f,  0.5f),
	glm::vec3( 0.5f, -0.5f,  0.5f),
	glm::vec3( 0.5f,  0.5f,  0.5f),
	glm::vec3( 0.5f, -0.5f, -0.5f), 
	glm::vec3( 0.5f,  0.5f, -0.5f),
	glm::vec3(-0.5f, -0.5f, -0.5f),
	glm::vec3(-0.5f,  0.5f, -0.5f), 
	glm::vec3( 0.5f, -0.5f, -0.5f),
	glm::vec3( 0.5f,  0.5f, -0.5f),	
	glm::vec3(-0.5f, -0.5f,  0.5f),
	glm::vec3(-0.5f,  0.5f,  0.5f),
	glm::vec3(-0.5f, -0.5f, -0.5f),	
	glm::vec3(-0.5f,  0.5f, -0.5f),
	glm::vec3(-0.5f,  0.5f,  0.5f), 
	glm::vec3( 0.5f,  0.5f,  0.5f), 
	glm::vec3(-0.5f,  0.5f, -0.5f), 
	glm::vec3( 0.5f,  0.5f, -0.5f), 
	glm::vec3(-0.5f, -0.5f,  0.5f),
	glm::vec3( 0.5f, -0.5f,  0.5f), 
	glm::vec3(-0.5f, -0.5f, -0.5f), 
	glm::vec3( 0.5f, -0.5f, -0.5f),
	};*/

	std::vector<glm::vec3> vertices = {
		// Back
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),

		// Front
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),

		// Left
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),

		// Right
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),

		// Bottom
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),

		// Top
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
	};

	std::vector<glm::vec3> normals =
	{
		 glm::vec3(0.0f, 0.0f, -1.0f),
		 glm::vec3(0.0f, 0.0f, -1.0f),
		 glm::vec3(0.0f, 0.0f, -1.0f),
		 glm::vec3(0.0f, 0.0f, -1.0f),
		 glm::vec3(0.0f, 0.0f, -1.0f),
		 glm::vec3(0.0f, 0.0f, -1.0f),

		 glm::vec3(0.0f, 0.0f, 1.0f),
		 glm::vec3(0.0f, 0.0f, 1.0f),
		 glm::vec3(0.0f, 0.0f, 1.0f),
		 glm::vec3(0.0f, 0.0f, 1.0f),
		 glm::vec3(0.0f, 0.0f, 1.0f),
		 glm::vec3(0.0f, 0.0f, 1.0f),

		 glm::vec3(-1.0f, 0.0f, 0.0f),
		 glm::vec3(-1.0f, 0.0f, 0.0f),
		 glm::vec3(-1.0f, 0.0f, 0.0f),
		 glm::vec3(-1.0f, 0.0f, 0.0f),
		 glm::vec3(-1.0f, 0.0f, 0.0f),
		 glm::vec3(-1.0f, 0.0f, 0.0f),

		 glm::vec3(1.0f, 0.0f, 0.0f),
		 glm::vec3(1.0f, 0.0f, 0.0f),
		 glm::vec3(1.0f, 0.0f, 0.0f),
		 glm::vec3(1.0f, 0.0f, 0.0f),
		 glm::vec3(1.0f, 0.0f, 0.0f),
		 glm::vec3(1.0f, 0.0f, 0.0f),

		 glm::vec3(0.0f, -1.0f, 0.0f),
		 glm::vec3(0.0f, -1.0f, 0.0f),
		 glm::vec3(0.0f, -1.0f, 0.0f),
		 glm::vec3(0.0f, -1.0f, 0.0f),
		 glm::vec3(0.0f, -1.0f, 0.0f),
		 glm::vec3(0.0f, -1.0f, 0.0f),

		 glm::vec3(0.0f, 1.0f, 0.0f),
		 glm::vec3(0.0f, 1.0f, 0.0f),
		 glm::vec3(0.0f, 1.0f, 0.0f),
		 glm::vec3(0.0f, 1.0f, 0.0f),
		 glm::vec3(0.0f, 1.0f, 0.0f),
		 glm::vec3(0.0f, 1.0f, 0.0f)
	};

	std::vector<glm::vec2> texCoords = {
		// Back
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(0.0f, 1.0f), // 1
		glm::vec2(1.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
		glm::vec2(1.0f, 0.0f), // 4
		glm::vec2(0.0f, 0.0f), // 5

		// Front
		glm::vec2(1.0f, 1.0f), // 2
		glm::vec2(0.0f, 1.0f), // 1
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(0.0f, 0.0f), // 5
		glm::vec2(1.0f, 0.0f), // 4
		glm::vec2(1.0f, 1.0f), // 3


		// Left
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(0.0f, 1.0f), // 1
		glm::vec2(1.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
		glm::vec2(1.0f, 0.0f), // 4
		glm::vec2(0.0f, 0.0f), // 5

		// Right
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(0.0f, 1.0f), // 1
		glm::vec2(1.0f, 1.0f), // 2
		glm::vec2(0.0f, 0.0f), // 5
		glm::vec2(1.0f, 0.0f), // 4
		glm::vec2(1.0f, 1.0f), // 3


		// Bottom
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(0.0f, 1.0f), // 1
		glm::vec2(1.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
		glm::vec2(1.0f, 0.0f), // 4
		glm::vec2(0.0f, 0.0f), // 5

		// Top
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(0.0f, 1.0f), // 1
		glm::vec2(1.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
		glm::vec2(1.0f, 0.0f), // 4
		glm::vec2(0.0f, 0.0f), // 5
	};

	/*std::vector<GLshort> indices = {
		2, 1, 0, 3, 1, 2, // Front
		6, 5, 4, 7, 5, 6, // Right
		8, 9, 10, 10, 9, 11, // Back
		12, 13, 14, 14, 13, 15, // Left
		16, 17, 18, 18, 17, 19, // Top
		22, 21, 20, 23, 21, 22  // Bottom
	};*/

	std::vector<GLshort> indices = {
		0, 1, 2, 3, 4, 5, // Front
		6, 7, 8, 9, 10, 11, // Right
		12, 13, 14, 15, 16, 17, // Back
		18, 19, 20, 21, 22, 23, // Left
		24, 25, 26, 27, 28, 29, // Top
		30, 31, 32, 33, 34, 35  // Bottom
	};

	SetVertices(vertices);
	SetIndices(indices);
	SetTextureCoords(texCoords);
	SetNormals(normals);

	//CalculateNormals();

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

std::vector<glm::vec3>& MMesh::GetNormals()
{
	return m_pNormals;
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
