#include "stdafx.h"
#include "msk-gl-renderer.h"

using namespace TexGen;

const char* vs = R"glsl(
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
		gl_PointSize = 5;
	}
)glsl";

const char* fs = R"glsl(
	#version 440 core

	// Output
	out vec4 outColor;

	void main()
	{
		outColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	} 
)glsl";

const char* fs_points = R"glsl(
	#version 440 core

	// Output
	out vec4 outColor;

	void main()
	{
		outColor = vec4(0.5f, 0.5f, 0.5f, 0.8f);
	} 
)glsl";

TgRenderer::TgRenderer() :
	m_baseVertex(0),
	m_baseFaceIndex(0),
	m_instanceBase(0),
	m_instanceEdgeBase(0),
	m_baseIndirectFaceIndex(0),
	m_baseIndirectEdgeIndex(0),
	m_baseEdgeIndex(0),
	m_baseModelMatrix(0),
	m_flush(false),
	m_drawing(false),
	m_numOfVertices(10000),
	m_numOfIndices(10000),
	m_numOfObjects(10000),
	m_numOfEdgeIndices(10000),
	m_vertexShader(eShaderType::Vertex),
	m_fragmentShader(eShaderType::Fragment),
	m_fragmentShaderPoints(eShaderType::Fragment)
{
	// Initalize the shaders
	initShaders();

	// Generate Vertex Attribute Object
	initBuffers();

	// Fill Buffers
	fillBuffers();
}

TgRenderer::~TgRenderer()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_cbo);
	glDeleteBuffers(1, &m_vao);
	glDeleteBuffers(1, &m_drawId);
	glDeleteBuffers(1, &m_indirectBO);
	glDeleteBuffers(1, &m_modelMatrixId);
}

GLvoid TgRenderer::setProjectionMatrix(const glm::mat4& matProj)
{
	m_matProjection = matProj;
}

GLvoid TgRenderer::addMesh(const TgMesh& mesh)
{
	dcel::manifold manifold = mesh.getManifold();

	// Generate vertices from mesh
	std::vector<dcel::vertex_attr> vertices = manifold.generate_vertices();

	// Generate face indices from mesh
	std::vector<unsigned int> faceIndices = manifold.generate_face_indices();

	// Generate vertex indices from mesh
	std::vector<unsigned int> edgeIndices = manifold.generate_edge_indices();

	// Get the model matrix
	glm::mat4 modelMatrix = mesh.getModelMatrix();

	DrawElementsCommand faceIndirect = DrawElementsCommand{
			faceIndices.size(), // Vertex Count
			1,							// Instance Count
			m_baseFaceIndex,			// FirstIndex
			m_baseVertex,				// BaseVertex
			m_instanceBase				// BaseInstance
		};

	DrawElementsCommand edgeIndirect = DrawElementsCommand{
			edgeIndices.size(), 			// Vertex Count
			1,							   // Instance Count
			m_baseEdgeIndex,			   // FirstIndex
			m_baseVertex,				   // BaseVertex
			m_instanceEdgeBase			   // BaseInstance
		};

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, m_baseVertex * sizeof(dcel::vertex_attr), vertices.size() * sizeof(dcel::vertex_attr), &vertices[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_baseModelMatrix * sizeof(glm::mat4), sizeof(glm::mat4), &modelMatrix);

	if (faceIndices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_baseFaceIndex * sizeof(GLuint), faceIndices.size() * sizeof(GLuint), &faceIndices[0]);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBO);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, m_baseIndirectFaceIndex * sizeof(DrawElementsCommand), sizeof(DrawElementsCommand), &faceIndirect);
		m_baseFaceIndex += faceIndices.size();
		m_baseIndirectFaceIndex++;
	}

	if (edgeIndices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_edgeIBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_baseEdgeIndex * sizeof(GLuint), edgeIndices.size() * sizeof(GLuint), &edgeIndices[0]);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectEdgeBO);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, m_baseIndirectEdgeIndex * sizeof(DrawElementsCommand), sizeof(DrawElementsCommand), &edgeIndirect);
		m_baseEdgeIndex += edgeIndices.size();
		m_baseIndirectEdgeIndex++;
	}

	glBindVertexArray(0); // Unbind
	m_baseVertex += vertices.size();
	m_baseModelMatrix++;
	m_instanceBase++;
	m_instanceEdgeBase++;
}

void TgRenderer::draw()
{
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);

	// ==========================================
	// Render triangles
	// ==========================================
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBO);

	m_shaderProgram.use();
	m_shaderProgram.setUniform("MVP", m_matProjection);

	glMultiDrawElementsIndirect(GL_TRIANGLES,
		GL_UNSIGNED_INT,
		nullptr,
		m_baseIndirectFaceIndex,
		0);

	// ==========================================
	//  Render edges
	// ==========================================
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_edgeIBO);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectEdgeBO);

	m_shaderProgramPoints.use();
	m_shaderProgramPoints.setUniform("MVP", m_matProjection);

	glMultiDrawElementsIndirect(GL_LINES,
		GL_UNSIGNED_INT,
		nullptr,
		m_baseIndirectEdgeIndex,
		0);
	// ==========================================
	//  Render points
	// ==========================================
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBO);
	
	glMultiDrawElementsIndirect(GL_POINTS,
		GL_UNSIGNED_INT,
		nullptr,
		m_baseIndirectFaceIndex,
		0);
	glClear(GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(0); // Unbind

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Disable shader
	m_shaderProgram.disable();
}

void TgRenderer::begin()
{
	if (m_drawing)
		throw std::invalid_argument("MRenderer.End() must be called before Begin().");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_drawing = true;
}

void TgRenderer::flush()
{
	glBindVertexArray(m_vao);

	// Return if no vertices present
	if (m_vertexData.size() == 0)
		return;

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(dcel::vertex_attr), &m_vertexData[0], GL_STATIC_DRAW);

	// Set vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(dcel::vertex_attr, pos)); // Position
	glVertexAttribBinding(0, 0);

	// Bind vertex buffers
	glBindVertexBuffer(0, m_vbo, 0, sizeof(dcel::vertex_attr));

	// SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_modelMatrices.size() * sizeof(glm::mat4), &m_modelMatrices[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	// ==================================================
	// Render Triangles
	// ==================================================

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

	m_shaderProgram.use();
	m_shaderProgram.setUniform("MVP", m_matProjection);

	// Render triangles
	glMultiDrawElementsIndirect(GL_TRIANGLES,
		GL_UNSIGNED_INT,
		nullptr,
		m_indirectData.size(),
		0);

	m_shaderProgramPoints.use();
	m_shaderProgramPoints.setUniform("MVP", m_matProjection);

	// ==================================================
	// Render lines
	// ==================================================

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_edgeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexEdgeData.size() * sizeof(uint32_t), &m_indexEdgeData[0], GL_STATIC_DRAW);

	// Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectEdgeBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_indirectEdgeData.size() * sizeof(DrawElementsCommand), &m_indirectEdgeData[0], GL_STATIC_DRAW);

	// DrawID
	glBindBuffer(GL_ARRAY_BUFFER, m_indirectEdgeBO);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(1, 1); // only once per instance

	// Render edges
	glMultiDrawElementsIndirect(GL_LINES,
		GL_UNSIGNED_INT,
		nullptr,
		m_indirectEdgeData.size(),
		0);

	// ==================================================
	// Render points
	// ==================================================
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_indirectBO);

	// Render points
	glMultiDrawElementsIndirect(GL_POINTS,
		GL_UNSIGNED_INT,
		nullptr,
		m_indirectData.size(),
		0);

	glBindVertexArray(0); // Unbind

	m_vertexData.clear();
	m_indirectData.clear();
	m_indirectEdgeData.clear();
	m_indexEdgeData.clear();
	m_indexData.clear();
	m_modelMatrices.clear();
	m_color.clear();
	m_drawIds.clear();
	m_baseVertex = 0;
	m_baseFaceIndex = 0;
	m_instanceBase = 0;
	m_flush = false;
}

void TgRenderer::end()
{
	if (!m_drawing)
		throw std::invalid_argument("MRenderer.Begin() must be called before End().");

	flush();

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Disable shader
	m_shaderProgram.disable();

	m_drawing = false;
}

void TgRenderer::draw(const TgMesh& mesh)
{
	// Store indices
	m_indexData.insert(m_indexData.end(), mesh.getIndices().begin(), mesh.getIndices().end());
	m_indexEdgeData.insert(m_indexEdgeData.end(), mesh.getEdgeIndices().begin(), mesh.getEdgeIndices().end());
	m_vertexData.insert(m_vertexData.end(), mesh.getVertices().begin(), mesh.getVertices().end());

	m_indirectData.push_back(DrawElementsCommand{
			(uint32_t)mesh.getIndices().size(),  // Vertex Count
			1,				 // Instance Count
			0,				 // FirstIndex
			m_baseVertex,	 // BaseVertex
			m_instanceBase	 // BaseInstance
		});

	m_indirectEdgeData.push_back(DrawElementsCommand{
			(uint32_t)mesh.getEdgeIndices().size(),  // Vertex Count
			1,				 // Instance Count
			m_baseFaceIndex,	 // FirstIndex
			m_baseVertex,   // BaseVertex
			m_instanceBase	 // BaseInstance
		});

	m_modelMatrices.push_back(mesh.getModelMatrix());
	m_baseVertex += mesh.getVertices().size();
	m_baseFaceIndex += mesh.getEdgeIndices().size();
	m_instanceBase++;

	if (m_flush) {
		flush();
	}
}

void TgRenderer::fillBuffers()
{
	glBindVertexArray(m_vao);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numOfVertices * sizeof(dcel::vertex_attr), nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(dcel::vertex_attr, pos)); // Position
	glVertexAttribBinding(0, 0);

	// Bind vertex buffers
	glBindVertexBuffer(0, m_vbo, 0, sizeof(dcel::vertex_attr));

	// Mesh SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_numOfObjects * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_ssbo);

	// Face Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numOfIndices* sizeof(GLuint), nullptr, GL_STATIC_DRAW);

	// Face Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_numOfObjects * sizeof(DrawElementsCommand), nullptr, GL_STATIC_DRAW);

	// Face DrawID
	glBindBuffer(GL_ARRAY_BUFFER, m_indirectBO);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT,  sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(1, 1); // only once per instance

	// Edge Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_edgeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numOfEdgeIndices * sizeof(uint32_t), nullptr, GL_STATIC_DRAW);

	// Edge Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectEdgeBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_numOfObjects * sizeof(DrawElementsCommand), nullptr, GL_STATIC_DRAW);

	// Edge DrawID
	glBindBuffer(GL_ARRAY_BUFFER, m_indirectEdgeBO);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(1, 1); // only once per instance

	glBindVertexArray(0); // Unbind
}

void TgRenderer::initShaders()
{
	// Load and compile shaders
	m_vertexShader.loadFromString(vs);
	m_vertexShader.compile();
	m_fragmentShader.loadFromString(fs);
	m_fragmentShader.compile();
	m_fragmentShaderPoints.loadFromString(fs_points);
	m_fragmentShaderPoints.compile();

	// Attach shaders
	m_shaderProgram.attachShader(m_vertexShader);
	m_shaderProgram.attachShader(m_fragmentShader);
	m_shaderProgram.link();
	m_shaderProgram.addUniform("MVP");

	m_shaderProgramPoints.attachShader(m_vertexShader);
	m_shaderProgramPoints.attachShader(m_fragmentShaderPoints);
	m_shaderProgramPoints.link();
	m_shaderProgramPoints.addUniform("MVP");
}

void TgRenderer::initBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_ssbo);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_edgeIBO);
	glGenBuffers(1, &m_cbo);
	glGenBuffers(1, &m_indirectBO);
	glGenBuffers(1, &m_indirectEdgeBO);
	glGenBuffers(1, &m_drawId);
	glGenBuffers(1, &m_modelMatrixId);
}
