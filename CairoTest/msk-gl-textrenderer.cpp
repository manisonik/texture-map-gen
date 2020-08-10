#include "stdafx.h"
#include "msk-gl-textrenderer.h"

const char* vs_textrenderer = R"glsl(
	#version 440 core
	layout (location = 0) in vec4 vertex;
	
	out vec2 TexCoords;
	uniform mat4 MVP;

	void main()
	{
		gl_Position = MVP * vec4(vertex.xy, 1.0, 1.0);
		TexCoords = vertex.zw;
	}
)glsl";

const char* fs_textrenderer = R"glsl(
	#version 440 core
	in vec2 TexCoords;
	out vec4 color;

	uniform sampler2D text;
	uniform vec3 textColor;

	void main()
	{    
		color = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r) * vec4(textColor, 1.0);
	}  
)glsl";

MTextRenderer::MTextRenderer() : 
	m_bDrawing(false)
{
	glGenVertexArrays(1, &m_hVAO);
	glGenBuffers(1, &m_hVBO);

	MskFactory::CreateShader(MskShaderType::Vertex, m_pVertexShader);
	MskFactory::CreateShader(MskShaderType::Fragment, m_pFragmentShader);
	MskFactory::CreateShaderProgram(m_pShaderProgram);

	m_pVertexShader->LoadFromString(vs_textrenderer);
	m_pVertexShader->Compile();
	m_pFragmentShader->LoadFromString(fs_textrenderer);
	m_pFragmentShader->Compile();

	m_pShaderProgram->AttachShader(m_pVertexShader);
	m_pShaderProgram->AttachShader(m_pFragmentShader);
	m_pShaderProgram->Link();

	// Add uniform
	m_pShaderProgram->AddUniform("textColor");
	m_pShaderProgram->AddUniform("MVP");
}

MTextRenderer::~MTextRenderer()
{
	// Release shaders
	m_pVertexShader.reset();
	m_pFragmentShader.reset();
	m_pShaderProgram.reset();

	// Delete buffers
	glDeleteBuffers(1, &m_hVBO);
	glDeleteBuffers(1, &m_hVAO);
}

void MTextRenderer::Begin(std::shared_ptr<MskFont>& mskFont)
{
	if (m_bDrawing)
		throw std::invalid_argument("CMSKOGLSpriteBatch.End() must be called before Begin().");

	// Store font
	m_pFont = mskFont;

	m_pShaderProgram->Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	// Set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mskFont->GetTextureId());

	// Set color
	m_pShaderProgram->SetUniform("textColor", 1.0f, 1.0f, 1.0f);

	m_bDrawing = true;
}

GLvoid MTextRenderer::SetProjectionMatrix(glm::mat4& matProj)
{
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	m_pShaderProgram->SetUniform("MVP", matProj);
}

void MTextRenderer::End()
{
	if (!m_bDrawing)
		throw std::invalid_argument("CMSKOGLSpriteBatch.Begin() must be called before End().");

	Flush();

	// Disable shader
	m_pShaderProgram->Disable();

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_bDrawing = false;
}

void MTextRenderer::Flush()
{
	if (m_vVertexData.size() <= 0)
		return;

	glBindVertexArray(m_hVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertexData.size() * sizeof(VertexPositionTexture), &m_vVertexData[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	// Render quad
	glDrawArrays(GL_TRIANGLES, 0, m_vVertexData.size());

	glBindVertexArray(0); // Unbind

	m_vVertexData.clear();
}

void MTextRenderer::RenderText(std::string text, float x, float y, float scale)
{
	for (auto c = text.begin(); c != text.end(); c++) {
		MskFontCharacter ch = m_pFont->GetCharacters()[*c];

		float x2 = x + ch.bl * scale;
		float y2 = -y - ch.bt * scale;
		float w = ch.bw * scale;
		float h = ch.bh * scale;

		/* Advance the cursor to the start of the next character */
		x += ch.ax * scale;
		y += ch.ay * scale;

		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;

		m_vVertexData.push_back({ glm::vec2(x2,     -y2 - h), glm::vec2(ch.tx, ch.bh / m_pFont->GetTextureHeight()) });
		m_vVertexData.push_back({ glm::vec2(x2 + w, -y2    ), glm::vec2(ch.tx + ch.bw / m_pFont->GetTextureWidth(), 0)});
		m_vVertexData.push_back({ glm::vec2(x2,     -y2), glm::vec2(ch.tx, 0) });
		m_vVertexData.push_back({ glm::vec2(x2 + w, -y2    ), glm::vec2(ch.tx + ch.bw / m_pFont->GetTextureWidth(), 0)});
		m_vVertexData.push_back({ glm::vec2(x2,     -y2 - h), glm::vec2(ch.tx, ch.bh / m_pFont->GetTextureHeight())});
		m_vVertexData.push_back({ glm::vec2(x2 + w, -y2 - h), glm::vec2(ch.tx + ch.bw / m_pFont->GetTextureWidth(), ch.bh / m_pFont->GetTextureHeight())});
	}
}
