#pragma once
#include "msk-gl-textureregion.h"

namespace TexGen {
	class TgSpriteBatch
	{
	private:
		struct DrawElementsCommand {
			unsigned int vertexCount;
			unsigned int instanceCount;
			unsigned int firstIndex;
			unsigned int baseVertex;
			unsigned int baseInstance;
		};

		struct VertexPositionTexture {
			glm::vec2 position;
			glm::vec2 texCoord;
		};

		unsigned int m_currentTextureId;
		unsigned int m_vbo;
		unsigned int m_ibo;
		unsigned int m_vao;
		unsigned int m_ssbo;
		unsigned int m_indirectBO;
		unsigned int m_drawId;
		bool m_flush;
		bool m_drawing;
		unsigned int m_baseVertex;
		unsigned int m_modelMatrixId;
		unsigned int m_programID;
		unsigned int m_instanceBase;
		glm::mat4 m_matProjection;

		TgShaderProgram m_shaderProgram;
		TgShader m_vertexShader;
		TgShader m_fragmentShader;
		std::vector<DrawElementsCommand> m_indirectData;
		std::vector<VertexPositionTexture> m_vertexData;
		std::vector<glm::mat4> m_modelMatrices;
		std::vector<GLuint> m_indexData;

		void initShaders();
		void initBuffers();
	public:
		TgSpriteBatch();
		~TgSpriteBatch();

		void setProjectionMatrix(const glm::mat4& matProj);
		void begin();
		void flush();
		void end();
		void draw(const TgTexture& texture, const glm::mat4& transformMatrix);
		void draw(const TgTextureRegion& region, float x, float y, float width, float height);
	};
}

