#pragma once
#include "msk-gl-shaderprogram.h"
#include "msk-core.h"

class MTextureRenderer : public MskTextureRenderer
{
private:
	struct VertexPositionColorTexture {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv;
	};

	struct DrawElementsCommand {
		GLuint vertexCount;
		GLuint instanceCount;
		GLuint firstIndex;
		GLuint baseVertex;
		GLuint baseInstance;
	};

	GLuint m_hVBO;
	GLuint m_hIBO;
	GLuint m_hNBO;
	GLuint m_hVAO;
	GLuint m_hCBO;
	GLuint m_hTBO;
	GLuint m_hIndirectBO;
	GLuint m_hDrawId;
	GLuint m_nBaseVertex;
	GLboolean m_bFlush;
	GLboolean m_bDrawing;
	GLuint m_hMVPMatrixId;
	GLuint m_hModelMatrixId;
	GLuint m_hProgramID;
	GLuint m_nVertexStride;
	GLuint m_nInstanceBase;
	GLboolean m_bLighting = false;

	// Store vertex data
	std::vector<glm::vec3> m_vVertexData;
	std::vector<glm::vec3> m_vNormalData;
	std::vector<glm::mat4> m_vModelMatrices;
	std::vector<glm::vec4> m_vColor;
	std::vector<glm::vec2> m_vTexCoords;
	std::vector<GLushort> m_vIndexData;
	std::vector<DrawElementsCommand> m_vIndirectData;
	std::vector<GLuint> m_vDrawId;
	std::vector<GLfloat> m_vVertices;

	// Vertex Attributes
	std::vector<MskVertexAttribute> m_pVertexAttributes;

	// Shaders
	std::shared_ptr<MskShader> m_pBasicTextureVS;
	std::shared_ptr<MskShader> m_pBasicTextureFS;
	std::shared_ptr<MskShaderProgram> m_pBasicTextureSP;
	std::shared_ptr<MskShader> m_pMaterialTextureVS;
	std::shared_ptr<MskShader> m_pMaterialTextureFS;
	std::shared_ptr<MskShaderProgram> m_pMaterialTextureSP;

public:
	MTextureRenderer();
	~MTextureRenderer();

	// MTextureRenderer Methods
	GLvoid InitShaders();
	GLvoid CalculateNormals(
		std::vector<GLshort>& indices,
		std::vector<glm::vec4>& pos, 
		std::vector<glm::vec3>& normals
	);

	GLvoid CalculateTangentBasis(
		std::vector<GLshort>& indices, 
		std::vector<glm::vec4>& pos, 
		std::vector<glm::vec3>& normals,
		std::vector<glm::vec2>& uv, 
		std::vector<glm::vec3>& tangents, 
		std::vector<glm::vec3>& bitangents
	);

	// MskTextureRenderer Methods
	GLvoid SetCamera(std::shared_ptr<MskCamera>& camera);
	GLvoid SetCamera(glm::mat4& matProj);
	GLvoid EnableLighting(GLboolean enable);
	GLvoid Draw(std::shared_ptr<MskMesh>& shape);
	GLvoid Begin();
	GLvoid Begin(std::shared_ptr<MskTexture>& texture);
	GLvoid End();
	GLvoid Flush();
};

