#pragma once
#include "msk-gl-shaderprogram.h"
#include "msk-gl-font.h"

namespace TexGen {
	class TgTextRenderer
	{
	private:
		struct VertexPositionTexture {
			glm::vec2 position;
			glm::vec2 texCoord;
		};

		uint32_t m_vbo;
		uint32_t m_currentTexture;
		uint32_t m_vao;
		bool m_drawing;
		TgShader m_vertexShader;
		TgShader m_fragmentShader;
		TgShaderProgram m_shaderProgram;
		TgFont m_font;
		std::vector<VertexPositionTexture> m_vertexData;
		std::vector<unsigned int> m_drawId;
		bool m_showCursor;
		float m_time;

	public:
		TgTextRenderer();
		virtual ~TgTextRenderer();

		void begin(const TgFont& font);
		void setProjectionMatrix(const glm::mat4& matProj);
		void end();
		void flush();
		void renderText(std::string text, float x, float y, float scale);
		void update(float dt);
		void windingOrder(unsigned int order);
	};
}

