#pragma once
#include "msk-gl-textureregion.h"

namespace Manisonik {
	class MSpriteBatch
	{
	private:
		struct DrawElementsCommand {
			GLuint vertexCount;
			GLuint instanceCount;
			GLuint firstIndex;
			GLuint baseVertex;
			GLuint baseInstance;
		};

		struct VertexPositionTexture {
			glm::vec2 position;
			glm::vec2 texCoord;
		};

		GLuint m_nCurrentTextureId;
		GLuint m_hVBO;
		GLuint m_hIBO;
		GLuint m_hVAO;
		GLuint m_hSSBO;
		GLuint m_hIndirectBO;
		GLuint m_hDrawId;
		GLboolean m_bFlush;
		GLboolean m_bDrawing;
		GLuint m_nBaseVertex;
		GLuint m_hModelMatrixId;
		GLuint m_hProgramID;
		GLuint m_nInstanceBase;
		glm::mat4 m_matProjection;

		std::shared_ptr<MShaderProgram> m_pShaderProgram;
		std::vector<DrawElementsCommand> m_vIndirectData;
		std::vector<VertexPositionTexture> m_vVertexData;
		std::vector<glm::mat4> m_vModelMatrices;
		std::vector<GLuint> m_vIndexData;

		void InitShaders();
		void InitBuffers();
	public:
		MSpriteBatch();
		~MSpriteBatch();

		void SetProjectionMatrix(glm::mat4& matProj);
		void Begin();
		void Flush();
		void End();
		void Draw(std::shared_ptr<MTexture>& texture, glm::mat4 transformMatrix);
		void Draw(std::shared_ptr<MTextureRegion>& region, float x, float y, float width, float height);
	};
}

