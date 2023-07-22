#pragma once
#include "msk-gl-shaderprogram.h"
#include "msk-event.h"

class MShapeRenderer : public MskShapeRenderer
{
private:
	struct VertexPositionColor {
		glm::vec4 position;
		glm::vec4 color;
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
	GLuint m_hVAO;
	GLuint m_hCBO;
	GLuint m_hIndirectBO;
	GLuint m_hDrawId;
	GLuint m_nBaseVertex;
	GLboolean m_bFlush;
	GLboolean m_bDrawing;
	GLuint m_hMVPMatrixId;
	GLuint m_hModelMatrixId;
	GLuint m_hProgramID;
	GLenum m_nPrimitiveType;
	GLuint m_nInstanceBase;

	// Store vertex data
	std::vector<glm::vec3> m_vVertexData;
	std::vector<GLuint> m_vIndexData;
	std::vector<DrawElementsCommand> m_vIndirectData;
	std::vector<GLuint> m_vDrawId;
	std::vector<glm::mat4> m_vModelMatrices;
	std::vector<glm::vec4> m_vColor;

	// Shaders
	std::shared_ptr<MskShader> m_pVertexShader;
	std::shared_ptr<MskShader> m_pFragmentShader;
	std::shared_ptr<MskShaderProgram> m_pShaderProgram;

	// Vertex data
	glm::vec4 m_V0, m_V1, m_V2, m_V3;

	GLvoid Reset();
public:
	MShapeRenderer();
	~MShapeRenderer();

	GLvoid InitShaders();
	GLvoid SetProjectionMatrix(glm::mat4& matProj);
	GLvoid Begin();
	GLvoid End();
	GLvoid Draw(std::shared_ptr<MskMesh>& mesh);
	GLvoid Flush();
};

