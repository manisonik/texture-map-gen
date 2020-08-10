#pragma once
#include "msk-core.h"

class MLineRenderer :
	public MskLineRenderer
{
private:
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
	GLuint m_hEdge0;
	GLuint m_hEdge1;
	GLuint m_hEdge2;
	GLuint m_hEdge3;
	GLuint m_hProgramID;
	GLenum m_nPrimitiveType;
	GLuint m_nLineSize;
	GLuint m_nInstanceBase;

	// Store vertex data
	std::vector<glm::vec3> m_vVertexData;
	std::vector<glm::vec4> m_vColor;
	std::vector<GLshort> m_vIndexData;
	std::vector<DrawElementsCommand> m_vIndirectData;
	std::vector<glm::mat4> m_vModelMatrices;

	// Shaders
	std::shared_ptr<MskShader> m_pVertexShader;
	std::shared_ptr<MskShader> m_pFragmentShader;
	std::shared_ptr<MskShaderProgram> m_pShaderProgram;

	// Camera
	std::shared_ptr<MskCamera> mskCamera;

public:
	MLineRenderer(MskApp* mskApp);
	~MLineRenderer();

	GLvoid InitShaders();
	GLvoid AntiAliasing(glm::vec3 p1, glm::vec3 p2, GLfloat radius);
	GLvoid SetProjectionMatrix(glm::mat4& matProj);
	GLvoid Begin();
	GLvoid End();
	GLvoid Draw(std::shared_ptr<MskMesh>& mesh);
	GLvoid Flush();
};

