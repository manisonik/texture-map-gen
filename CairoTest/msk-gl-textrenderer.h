#pragma once
#include "msk-core.h"

class MTextRenderer : public MskTextRenderer
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
	std::shared_ptr<MskShader> m_pVertexShader;
	std::shared_ptr<MskShader> m_pFragmentShader;
	std::shared_ptr<MskShaderProgram> m_pShaderProgram;
	std::shared_ptr<MskFont> m_pFont;
	std::vector<VertexPositionTexture> m_vVertexData;
	std::vector<GLuint> m_vDrawId;

public:
	MTextRenderer();
	virtual ~MTextRenderer();

	GLvoid Begin(std::shared_ptr<MskFont>& mskFont);
	GLvoid SetProjectionMatrix(glm::mat4 & matProj);
	GLvoid End();
	GLvoid Flush();
	GLvoid RenderText(std::string text, float x, float y, float scale);
};

