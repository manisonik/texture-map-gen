#include "stdafx.h"
#include "msk-gl-line-renderer.h"

using namespace TexGen;

const char* lineRenderer_vs = R"glsl(
	#version 440 core

		// Input vertex data, different for all executions of this shader.
	layout(location = 0) in vec3 vertexPosition_modelspace;
	layout(location = 1) in uint drawId;
  
	layout(std430, binding = 2) buffer modelMatrices
	{
		mat4 instanceMatrix[];
	};

	// Values that stay constant for the whole mesh.
	uniform mat4 MVP;

	void main()
	{
		gl_Position = MVP * instanceMatrix[drawId] * vec4(vertexPosition_modelspace, 1.0f);
	}
)glsl";

const char* lineRenderer_fs = R"glsl(
	#version 440 core

	// Output
	out vec4 outColor;

	void main()
	{
		outColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	} 
)glsl";

TgLineRenderer::TgLineRenderer() :
	m_baseVertex(0),
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
}

TgLineRenderer::~TgLineRenderer()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_cbo);
	glDeleteBuffers(1, &m_vao);
	glDeleteBuffers(1, &m_drawId);
	glDeleteBuffers(1, &m_indirectBO);
	glDeleteBuffers(1, &m_modelMatrixId);
}

void TgLineRenderer::setProjectionMatrix(const glm::mat4& matProj)
{
	m_shaderProgram.setUniform("MVP", matProj);
}

void TgLineRenderer::begin()
{
	if (m_drawing)
		throw std::invalid_argument("MShapeRenderer.End() must be called before Begin().");

	m_shaderProgram.use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_drawing = true;
}

void TgLineRenderer::flush()
{
	glBindVertexArray(m_vao);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(dcel::vertex_attr), &m_vertexData[0], GL_STATIC_DRAW);

	// Set vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(dcel::vertex_attr, pos)); // Position
	glVertexAttribBinding(0, 0);

	// Bind vertex buffers
	glBindVertexBuffer(0, m_vbo, 0, sizeof(dcel::vertex_attr));

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

	// SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_modelMatrices.size() * sizeof(glm::mat4), &m_modelMatrices[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	glMultiDrawElementsIndirect(GL_LINES,
		GL_UNSIGNED_INT,
		nullptr,
		m_indirectData.size(),
		0);

	glBindVertexArray(0); // Unbind

	m_vertexData.clear();
	m_indirectData.clear();
	m_indexData.clear();
	m_modelMatrices.clear();
	m_color.clear();
	m_drawIds.clear();
	m_baseVertex = 0;
	m_instanceBase = 0;
	m_flush = false;
}

void TgLineRenderer::end()
{
	if (!m_drawing)
		throw std::invalid_argument("MShapeRenderer.Begin() must be called before End().");

	flush();

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Disable shader
	m_shaderProgram.disable();

	m_drawing = false;
}

void TgLineRenderer::draw(const TgMesh& mesh)
{
	// Store indices
	m_indexData.insert(m_indexData.end(), mesh.getEdgeIndices().begin(), mesh.getEdgeIndices().end());
	m_vertexData.insert(m_vertexData.end(), mesh.getVertices().begin(), mesh.getVertices().end());


	auto element = std::make_shared<DrawElementsCommand>();
	element->vertexCount = mesh.getIndices().size();
	element->instanceCount = 1;
	element->firstIndex = 0;
	element->baseVertex = m_baseVertex;
	element->baseInstance = m_instanceBase;
	m_indirectData.push_back(element);

	m_modelMatrices.push_back(mesh.getModelMatrix());
	m_baseVertex += mesh.getVertices().size();
	m_instanceBase++;

	if (m_flush) {
		flush();
	}
}

GLvoid TgLineRenderer::initShaders()
{
	// Load and compile shaders
	m_vertexShader.loadFromString(lineRenderer_vs);
	m_vertexShader.compile();
	m_fragmentShader.loadFromString(lineRenderer_fs);
	m_fragmentShader.compile();

	// Attach shaders
	m_shaderProgram.attachShader(m_vertexShader);
	m_shaderProgram.attachShader(m_fragmentShader);
	m_shaderProgram.link();

	m_shaderProgram.addUniform("MVP");
}

void TgLineRenderer::initBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_ssbo);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_cbo);
	glGenBuffers(1, &m_indirectBO);
	glGenBuffers(1, &m_drawId);
	glGenBuffers(1, &m_modelMatrixId);
}
