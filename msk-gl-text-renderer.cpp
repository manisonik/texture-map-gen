#include "stdafx.h"
#include "msk-gl-text-renderer.h"

using namespace TexGen;

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
	//uniform vec3 textColor;

	void main()
	{    
		color = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r) * vec4(1.0, 1.0, 1.0, 1.0);
	}  
)glsl";

TgTextRenderer::TgTextRenderer() :
	m_drawing(false),
	m_time(0),
	m_vertexShader(eShaderType::Vertex),
	m_fragmentShader(eShaderType::Fragment)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// Load and compile shaders
	m_vertexShader.loadFromString(vs_textrenderer);
	m_vertexShader.compile();
	m_fragmentShader.loadFromString(fs_textrenderer);
	m_fragmentShader.compile();

	// Attach shaders
	m_shaderProgram.attachShader(m_vertexShader);
	m_shaderProgram.attachShader(m_fragmentShader);
	m_shaderProgram.link();

	m_shaderProgram.addUniform("MVP");
}

TgTextRenderer::~TgTextRenderer()
{
	// Delete buffers
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_vao);
}

void TgTextRenderer::begin(const TgFont& font)
{
	if (m_drawing)
		throw std::invalid_argument("CMSKOGLSpriteBatch.End() must be called before Begin().");

	// Store font
	m_font = font;

	m_shaderProgram.use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	// Set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.getTextureId());

	// Set color
	//m_shaderProgram.setUniform("textColor", 1.0f, 1.0f, 1.0f);

	m_drawing = true;
}

void TgTextRenderer::setProjectionMatrix(const glm::mat4& matProj)
{
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	m_shaderProgram.setUniform("MVP", matProj);
}

void TgTextRenderer::end()
{
	if (!m_drawing)
		throw std::invalid_argument("CMSKOGLSpriteBatch.Begin() must be called before End().");

	flush();

	// Disable shader
	m_shaderProgram.disable();

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_drawing = false;
}

void TgTextRenderer::flush()
{
	if (m_vertexData.size() <= 0)
		return;

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(VertexPositionTexture), &m_vertexData[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	// Render quad
	glDrawArrays(GL_TRIANGLES, 0, m_vertexData.size());

	glBindVertexArray(0); // Unbind

	m_vertexData.clear();
}

void TgTextRenderer::update(float dt)
{
	m_time += dt;

	if (m_time > 0.4) {
		m_time = 0;
	}
	if (m_time > 0.2) {
		m_showCursor = false;
	}
	else {
		m_showCursor = true;
	}
}

void TgTextRenderer::windingOrder(unsigned int order)
{
	// Top-Left
}

void TgTextRenderer::renderText(std::string text, float x, float y, float scale)
{
	if (m_showCursor)
		text.append("|");

	for (auto c = text.begin(); c != text.end(); c++) {
		auto ch = m_font.getCharacters()[*c];

		float x2 = x + ch->bl * scale;
		float y2 = -y - ch->bt * scale;
		float w = ch->bw * scale;
		float h = ch->bh * scale;

		/* Advance the cursor to the start of the next character */
		x += ch->ax * scale;
		y += ch->ay * scale;

		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;
		
		m_vertexData.push_back({ glm::vec2(x2,     -y2), glm::vec2(ch->tx, 0) });
		m_vertexData.push_back({ glm::vec2(x2 + w, -y2), glm::vec2(ch->tx + ch->bw / m_font.getTextureWidth(), 0) });
		m_vertexData.push_back({ glm::vec2(x2,     -y2 - h), glm::vec2(ch->tx, ch->bh / m_font.getTextureHeight()) });
		m_vertexData.push_back({ glm::vec2(x2 + w, -y2 - h), glm::vec2(ch->tx + ch->bw / m_font.getTextureWidth(), ch->bh / m_font.getTextureHeight()) });
		m_vertexData.push_back({ glm::vec2(x2,     -y2 - h), glm::vec2(ch->tx, ch->bh / m_font.getTextureHeight()) });
		m_vertexData.push_back({ glm::vec2(x2 + w, -y2), glm::vec2(ch->tx + ch->bw / m_font.getTextureWidth(), 0) });
		
		//m_vVertexData.push_back({ glm::vec2(x2,     -y2 - h), glm::vec2(ch.tx, 0) });
		//m_vVertexData.push_back({ glm::vec2(x2 + w, -y2), glm::vec2(ch.tx + ch.bw / m_pFont->GetTextureWidth(), ch.bh / m_pFont->GetTextureHeight()) });
		//m_vVertexData.push_back({ glm::vec2(x2,     -y2),  glm::vec2(ch.tx, ch.bh / m_pFont->GetTextureHeight()) });
		//m_vVertexData.push_back({ glm::vec2(x2 + w, -y2), glm::vec2(ch.tx + ch.bw / m_pFont->GetTextureWidth(), ch.bh / m_pFont->GetTextureHeight()) });
		//m_vVertexData.push_back({ glm::vec2(x2,     -y2 - h), glm::vec2(ch.tx, 0) });
		//m_vVertexData.push_back({ glm::vec2(x2 + w, -y2 - h), glm::vec2(ch.tx + ch.bw / m_pFont->GetTextureWidth(), 0) });
	}
}