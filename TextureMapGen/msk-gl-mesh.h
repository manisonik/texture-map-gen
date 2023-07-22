#pragma once
#include "msk-core.h"

class MMesh :
	public MskMesh
{
private:
	struct Subset {
		GLuint baseVertex;
		GLuint vertexCount;
	};

	std::vector<MskFace> m_pFaces;
	std::vector<Subset> m_pSubset;
	std::vector<GLuint> m_pIndices;
	std::vector<glm::vec3> m_pVertices;
	std::vector<glm::vec3> m_pNormals;
	std::vector<glm::vec3> m_pTangents;
	std::vector<glm::vec3> m_pBitangents;
	std::vector<glm::vec2> m_pTexCoords;
	std::vector<MskVertexAttribute> m_pVertexAttributes;
	glm::mat4 m_matScale;
	glm::mat4 m_matTranslate;
	glm::mat4 m_matRotate;
	glm::mat4 m_matModel;
	glm::mat4 m_matIdentity;
	GLuint m_nTotalVertexComponents;
	glm::vec4 m_vColor;
	GLuint m_nBaseVertex;
public:
	MMesh();
	virtual ~MMesh();

	GLvoid SetColor(glm::vec4 color);
	GLvoid AddPlane();
	GLvoid AddGrid(GLfloat width, GLfloat height, GLfloat count);
	GLvoid AddCyclinder();
	GLvoid AddCone();
	GLvoid AddCube();
	GLvoid SetFaces(std::vector<MskFace>& faces);
	GLvoid SetVertices(std::vector<glm::vec3>& vertices);
	GLvoid SetNormals(std::vector<glm::vec3>& normals);
	GLvoid SetIndices(std::vector<GLuint>& indices);
	GLvoid SetTextureCoords(std::vector<glm::vec2>& texCoords);
	GLvoid SetVertexAttributes(std::vector<MskVertexAttribute>& vertexAttributes);
	GLvoid Rotate(GLfloat angle);
	GLvoid Translate(GLfloat dx, GLfloat dy, GLfloat dz);
	GLvoid Translate(GLuint subset, GLfloat dx, GLfloat dy, GLfloat dz);
	GLvoid Scale(GLfloat x, GLfloat y, GLfloat z);
	GLvoid Update();
	glm::mat4& GetModelMatrix();
	std::vector<MskFace>& GetFaces();
	std::vector<glm::vec3>& GetNormals();
	std::vector<glm::vec3>& GetTangents();
	std::vector<glm::vec3>& GetBitangents();
	std::vector<glm::vec3>& GetVertices();
	std::vector<GLuint>& GetIndices();
	std::vector<glm::vec2>& GetTextureCoords();
	glm::vec4& GetColor();
	GLvoid CalculateNormals();
	GLvoid CalculateTangentBasis();
};

