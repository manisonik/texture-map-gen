#pragma once
#include "msk-gl-shaderprogram.h"
#include "msk-gl-texture.h"

namespace TexGen {
	struct ITgMultisampleFrameRenderer
	{
		virtual void begin() = 0;
		virtual void end() = 0;
		virtual ~ITgMultisampleFrameRenderer() = default;
	};

	class TgMultisampleFrameRenderer : public ITgMultisampleFrameRenderer
	{
	private:
		// Shaders
		TgShaderProgram m_shaderProgram;
		TgTexture m_texture;
		TgTextureData m_textureData;
		std::vector<glm::vec3> m_vertexData;
		std::vector<uint32_t> m_indexData;
		std::vector<glm::vec2> m_texCoords;
		uint32_t m_rbo;
		uint32_t m_fboColor;
		uint32_t m_fboMSAA;
		uint32_t m_fboDepth;
		uint32_t m_fbo;
		uint32_t m_depthBuffer;
		int32_t m_maxSamples = 0;
		uint32_t m_width = 1024;
		uint32_t m_height = 1024;
		uint32_t m_samples = 4;
		uint32_t m_vao;
		uint32_t m_tbo;
		uint32_t m_vbo;
		uint32_t m_ibo;

		void initTextures();
		void initFrameBuffer();
		void initShaders();
		void initBuffers();
		void fillBuffers();

	public:
		TgMultisampleFrameRenderer();
		virtual ~TgMultisampleFrameRenderer();

		void begin() override;
		void end() override;
		void resize(int width, int height);
		const TgTexture& getTexture();
	};
}

