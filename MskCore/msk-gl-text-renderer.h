#pragma once
#include "msk-gl-shaderprogram.h"
#include "msk-gl-font.h"

namespace Manisonik {
	class MTextRenderer
	{
	private:
		struct VertexPositionTexture {
			glm::vec2 position;
			glm::vec2 texCoord;
		};

		GLuint m_hVBO;
		GLuint m_hCurrentTexture;
		GLuint m_hVAO;
		bool m_bDrawing;
		std::shared_ptr<MShader> m_pVertexShader;
		std::shared_ptr<MShader> m_pFragmentShader;
		std::shared_ptr<MShaderProgram> m_pShaderProgram;
		std::shared_ptr<MFont> m_pFont;
		std::vector<VertexPositionTexture> m_vVertexData;
		std::vector<GLuint> m_vDrawId;
		bool m_bShowCursor;
		float m_nTime;

	public:
		MTextRenderer();
		virtual ~MTextRenderer();

		GLvoid Begin(std::shared_ptr<MFont>& mskFont);
		GLvoid SetProjectionMatrix(glm::mat4& matProj);
		GLvoid End();
		GLvoid Flush();
		GLvoid RenderText(std::string text, float x, float y, float scale);
		void Update(float dt);
		void WindingOrder(uint32_t order);
	};
}

