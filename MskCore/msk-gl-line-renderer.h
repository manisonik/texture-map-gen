#pragma once
#include "msk-gl-mesh.h"
#include "msk-gl-shaderprogram.h"

namespace Manisonik {
	class MLineRenderer
	{
	public:
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
		GLuint m_hSSBO;
		GLuint m_hIndirectBO;
		GLuint m_hDrawId;
		GLuint m_nBaseVertex;
		GLboolean m_bFlush;
		GLboolean m_bDrawing;
		GLuint m_hModelMatrixId;
		GLuint m_nInstanceBase;

		// Store vertex data
		std::shared_ptr<MShaderProgram> m_pShaderProgram;
		std::vector<dcel::vertex_attr> m_vVertexData;
		std::vector<GLuint> m_vIndexData;
		std::vector<DrawElementsCommand> m_vIndirectData;
		std::vector<GLuint> m_vDrawId;
		std::vector<glm::mat4> m_vModelMatrices;
		std::vector<glm::vec4> m_vColor;

		MLineRenderer();
		~MLineRenderer();

		GLvoid SetProjectionMatrix(glm::mat4& matProj);
		GLvoid Begin();
		GLvoid Flush();
		GLvoid End();
		GLvoid Draw(std::shared_ptr<MMesh>& mesh);
		GLvoid InitShaders();
		GLvoid InitBuffers();
	};
}


