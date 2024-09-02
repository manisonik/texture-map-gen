#include "stdafx.h"
#include "msk-gl-spritebatch.h"

using namespace TexGen;

const char* vs_spritebatch = R"glsl(
	#version 440 core

	// Input vertex data, different for all executions of this shader.
	layout(location = 0) in vec4 vertex;
	layout(location = 1) in uint drawId;

	// Values that stay constant for the whole mesh.
	uniform mat4 MVP;

	out vec2 texCoords;

	void main()
	{
		gl_Position = MVP * vec4(vertex.xy, 1.0f, 1.0f);
		texCoords = vertex.zw;
	}
)glsl";

const char* fs_spritebatch = R"glsl(
	#version 440 core

	// Input texcoords
	in vec2 texCoords;

	uniform sampler2D tex;

	// Output
	out vec4 outColor;

	void main()
	{
		outColor = texture(tex, texCoords);
	} 
)glsl";

TgSpriteBatch::TgSpriteBatch() :
	m_baseVertex(0),
	m_currentTextureId(0),
	m_instanceBase(0),
	m_flush(false),
	m_drawing(false),
	m_vertexShader(eShaderType::Vertex),
	m_fragmentShader(eShaderType::Fragment)
{
	// Initalize the shaders
	initShaders();

	// Generate Vertex Attribute Object
	initBuffers();

	// Add index data
	m_indexData.insert(m_indexData.begin(), { 0, 1, 2, 2, 3, 0 });
}

TgSpriteBatch::~TgSpriteBatch()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vao);
	glDeleteBuffers(1, &m_indirectBO);
	glDeleteBuffers(1, &m_modelMatrixId);
}

void TgSpriteBatch::setProjectionMatrix(const glm::mat4& matProj)
{
	m_matProjection = matProj;
}

void TgSpriteBatch::begin()
{
	if (m_drawing)
		throw std::invalid_argument("MRenderer.End() must be called before Begin().");
	    		
	m_shaderProgram.use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_TEXTURE_2D);
	GLenum error = glGetError();

	m_drawing = true;
}

void TgSpriteBatch::flush()
{
	m_shaderProgram.setUniform("MVP", m_matProjection);		

	glBindVertexArray(m_vao);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(VertexPositionTexture), &m_vertexData[0], GL_STATIC_DRAW);

	// Set vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 4, GL_FLOAT, false, offsetof(VertexPositionTexture, position)); // Position
	glVertexAttribBinding(0, 0);

	// Bind vertex buffers
	glBindVertexBuffer(0, m_vbo, 0, sizeof(VertexPositionTexture));

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size() * sizeof(GLuint), &m_indexData[0], GL_STATIC_DRAW);

	// Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_indirectData.size() * sizeof(DrawElementsCommand), &m_indirectData[0], GL_STATIC_DRAW);

	// DrawID
	glBindBuffer(GL_ARRAY_BUFFER, m_indirectBO);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(1, 1); // only once per instance

	// Render triangles
	glMultiDrawElementsIndirect(GL_TRIANGLES,
		GL_UNSIGNED_INT,
		nullptr,
		m_indirectData.size(),
		0);
	
	glBindVertexArray(0); // Unbind

	m_vertexData.clear();
	m_indirectData.clear();
	m_modelMatrices.clear();
	m_baseVertex = 0;
	m_instanceBase = 0;
	m_flush = false;
}

void TgSpriteBatch::end()
{
	if (!m_drawing)
		throw std::invalid_argument("MRenderer.Begin() must be called before End().");

	flush();

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);

	// Disable shader
	m_shaderProgram.disable();

	m_drawing = false;
}

void TgSpriteBatch::draw(const TgTexture& texture, const glm::mat4& transformMatrix)
{
	// Create quad
	m_vertexData.push_back({ glm::vec2(50.0f, -50.0f), glm::vec2(1.0f, 0.0f) }); // 0
	m_vertexData.push_back({ glm::vec2(-50.0f, -50.0f), glm::vec2(0.0f, 0.0f) }); // 1
	m_vertexData.push_back({ glm::vec2(-50.0f, 50.0f), glm::vec2(0.0f, 1.0f) }); // 2
	m_vertexData.push_back({ glm::vec2(50.0f, 50.0f),glm::vec2(1.0f, 1.0f) }); // 3

	m_indirectData.push_back(DrawElementsCommand{
			(unsigned int)m_indexData.size(),  // Vertex Count
			1,							// Instance Count
			0,							// FirstIndex
			m_baseVertex,				// BaseVertex
			m_instanceBase				// BaseInstance
		});

	//m_modelMatrices.push_back(transformMatrix);
	m_baseVertex += 4;
	m_instanceBase++;

	// Bind texture
	texture.bind();

	if (m_flush) {
		flush();
	}
}

void TgSpriteBatch::draw(const TgTextureRegion& region, float x, float y, float width, float height)
{
	// Flush if we are using a different texture
	if (m_currentTextureId != 0 && m_currentTextureId != region.getTexture().getTextureId()) {
		flush();
	}

	// Create quad
	m_vertexData.push_back({ glm::vec2(x + width, y), glm::vec2(region.getU2(), region.getV()) }); // 0
	m_vertexData.push_back({ glm::vec2(x, y), glm::vec2(region.getU(), region.getV()) }); // 1
	m_vertexData.push_back({ glm::vec2(x, y + height), glm::vec2(region.getU(), region.getV2()) }); // 2
	m_vertexData.push_back({ glm::vec2(x + width, y + height),glm::vec2(region.getU2(), region.getV2()) }); // 3

	m_indirectData.push_back(DrawElementsCommand{
			(unsigned int)m_indexData.size(),	// Vertex Count
			1,						// Instance Count
			0,						// FirstIndex
			m_baseVertex,			// BaseVertex
			m_instanceBase			// BaseInstance
		});

	m_baseVertex += 4;
	m_instanceBase++;

	// Bind texture
	region.getTexture().bind();

	// Set current texture id
	m_currentTextureId = region.getTexture().getTextureId();
}

void TgSpriteBatch::initShaders()
{
	// Load and compile shaders
	m_vertexShader.loadFromString(vs_spritebatch);
	m_vertexShader.compile();
	m_fragmentShader.loadFromString(fs_spritebatch);
	m_fragmentShader.compile();

	// Attach shaders
	m_shaderProgram.attachShader(m_vertexShader);
	m_shaderProgram.attachShader(m_fragmentShader);
	m_shaderProgram.link();

	m_shaderProgram.addUniform("MVP");
}

void TgSpriteBatch::initBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_ssbo);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_indirectBO);
	glGenBuffers(1, &m_drawId);
	glGenBuffers(1, &m_modelMatrixId);
}
