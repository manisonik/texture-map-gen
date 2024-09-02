#pragma once
#include "msk-gl-mesh.h"
#include "msk-gl-shaderprogram.h"

namespace TexGen {
	class TgLineRenderer
	{
	public:
		struct DrawElementsCommand {
			uint32_t vertexCount;
			uint32_t instanceCount;
			uint32_t firstIndex;
			uint32_t baseVertex;
			uint32_t baseInstance;
		};

		uint32_t m_vbo;
		uint32_t m_ibo;
		uint32_t m_vao;
		uint32_t m_cbo;
		uint32_t m_ssbo;
		uint32_t m_indirectBO;
		uint32_t m_drawId;
		uint32_t m_baseVertex;
		bool m_flush;
		bool m_drawing;
		uint32_t m_modelMatrixId;
		uint32_t m_instanceBase;

		// Store vertex data
		TgShaderProgram m_shaderProgram;
		TgShader m_vertexShader;
		TgShader m_fragmentShader;
		std::vector<dcel::vertex_attr> m_vertexData;
		std::vector<uint32_t> m_indexData;
		std::vector<std::shared_ptr<DrawElementsCommand>> m_indirectData;
		std::vector<uint32_t> m_drawIds;
		std::vector<glm::mat4> m_modelMatrices;
		std::vector<glm::vec4> m_color;

		TgLineRenderer();
		~TgLineRenderer();

		void setProjectionMatrix(const glm::mat4& matProj);
		void begin();
		void flush();
		void end();
		void draw(const TgMesh& mesh);
		void initShaders();
		void initBuffers();
	};
}


