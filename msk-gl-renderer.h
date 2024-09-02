#pragma once
#include "msk-gl-mesh.h"
#include "msk-gl-shaderprogram.h"

namespace TexGen {
	class TgRenderer
	{
	public:
		struct DrawElementsCommand {
			uint32_t vertexCount;
			uint32_t instanceCount;
			uint32_t firstIndex;
			uint32_t baseVertex;
			uint32_t baseInstance;
		};

		uint32_t m_numOfVertices;
		uint32_t m_numOfObjects;
		uint32_t m_numOfIndices;
		uint32_t m_numOfEdgeIndices;

		uint32_t m_vbo;
		uint32_t m_ibo;
		uint32_t m_edgeIBO;
		uint32_t m_vao;
		uint32_t m_cbo;
		uint32_t m_ssbo;
		uint32_t m_indirectBO;
		uint32_t m_indirectEdgeBO;
		uint32_t m_drawId;
		uint32_t m_baseVertex;
		uint32_t m_baseIndirectFaceIndex;
		uint32_t m_baseIndirectEdgeIndex;
		uint32_t m_baseFaceIndex;
		uint32_t m_baseEdgeIndex;
		bool m_flush;
		bool m_drawing;
		uint32_t m_modelMatrixId;
		uint32_t m_programID;
		uint32_t m_instanceBase;
		uint32_t m_instanceEdgeBase;
		uint32_t m_baseModelMatrix;
		glm::mat4 m_matProjection;

		// Store vertex data
		TgShaderProgram m_shaderProgram;
		TgShaderProgram m_shaderProgramPoints;
		TgShader m_vertexShader;
		TgShader m_fragmentShader;
		TgShader m_fragmentShaderPoints;
		std::vector<dcel::vertex_attr> m_vertexData;
		std::vector<uint32_t> m_indexData;
		std::vector<uint32_t> m_indexEdgeData;
		std::vector<DrawElementsCommand> m_indirectData;
		std::vector<DrawElementsCommand> m_indirectEdgeData;
		std::vector<uint32_t> m_drawIds;
		std::vector<glm::mat4> m_modelMatrices;
		std::vector<glm::vec4> m_color;

		TgRenderer();
		~TgRenderer();

		void setProjectionMatrix(const glm::mat4& matProj);
		void draw();
		void addMesh(const TgMesh& mesh);
		void begin();
		void flush();
		void end();
		void draw(const TgMesh& mesh);
		void fillBuffers();
		void initShaders();
		void initBuffers();
	};
}

