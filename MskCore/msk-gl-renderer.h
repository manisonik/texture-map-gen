#pragma once
#include "msk-gl-mesh.h"
#include "msk-gl-shaderprogram.h"

namespace Manisonik {
	class MRenderer
	{
	public:
		struct DrawElementsCommand {
			GLuint vertexCount;
			GLuint instanceCount;
			GLuint firstIndex;
			GLuint baseVertex;
			GLuint baseInstance;
		};

		GLuint m_nNumOfVertices;
		GLuint m_nNumOfObjects;
		GLuint m_nNumOfIndices;
		GLuint m_nNumOfEdgeIndices;

		GLuint m_hVBO;
		GLuint m_hIBO;
		GLuint m_hEdgeIBO;
		GLuint m_hVAO;
		GLuint m_hCBO;
		GLuint m_hSSBO;
		GLuint m_hIndirectBO;
		GLuint m_hIndirectEdgeBO;
		GLuint m_hDrawId;
		GLuint m_nBaseVertex;
		GLuint m_nBaseIndirectFaceIndex;
		GLuint m_nBaseIndirectEdgeIndex;
		GLuint m_nBaseFaceIndex;
		GLuint m_nBaseEdgeIndex;
		GLboolean m_bFlush;
		GLboolean m_bDrawing;
		GLuint m_hModelMatrixId;
		GLuint m_hProgramID;
		GLuint m_nInstanceBase;
		GLuint m_nInstanceEdgeBase;
		GLuint m_nBaseModelMatrix;
		glm::mat4 m_matProjection;

		// Store vertex data
		std::shared_ptr<MShaderProgram> m_pShaderProgram;
		std::shared_ptr<MShaderProgram> m_pShaderProgramPoints;
		std::vector<dcel::vertex_attr> m_vVertexData;
		std::vector<GLuint> m_vIndexData;
		std::vector<GLuint> m_vIndexEdgeData;
		std::vector<DrawElementsCommand> m_vIndirectData;
		std::vector<DrawElementsCommand> m_vIndirectEdgeData;
		std::vector<GLuint> m_vDrawId;
		std::vector<glm::mat4> m_vModelMatrices;
		std::vector<glm::vec4> m_vColor;

		MRenderer();
		~MRenderer();

		GLvoid SetProjectionMatrix(glm::mat4& matProj);
		GLvoid Draw();
		GLvoid AddMesh(std::shared_ptr<MMesh>& mesh);
		GLvoid Begin();
		GLvoid Flush();
		GLvoid End();
		GLvoid Draw(std::shared_ptr<MMesh>& mesh);
		GLvoid FillBuffers();
		GLvoid InitShaders();
		GLvoid InitBuffers();
	};
}

