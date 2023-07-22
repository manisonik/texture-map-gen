#include "stdafx.h"
#include "msk-gl-line-renderer.h"

using namespace Manisonik;

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

MLineRenderer::MLineRenderer()
{
	// Set index to 0
	m_nBaseVertex = 0;

	// Set index to 0
	m_nInstanceBase = 0;

	// Set flush to false
	m_bFlush = false;

	// Set drawing to false
	m_bDrawing = false;

	// Initalize the shaders
	InitShaders();

	// Generate Vertex Attribute Object
	InitBuffers();
}

MLineRenderer::~MLineRenderer()
{
	glDeleteBuffers(1, &m_hVBO);
	glDeleteBuffers(1, &m_hIBO);
	glDeleteBuffers(1, &m_hCBO);
	glDeleteBuffers(1, &m_hVAO);
	glDeleteBuffers(1, &m_hDrawId);
	glDeleteBuffers(1, &m_hIndirectBO);
	glDeleteBuffers(1, &m_hModelMatrixId);
}

GLvoid MLineRenderer::SetProjectionMatrix(glm::mat4& matProj)
{
	m_pShaderProgram->SetUniform("MVP", matProj);
}

GLvoid MLineRenderer::Begin()
{
	if (m_bDrawing)
		throw std::invalid_argument("MShapeRenderer.End() must be called before Begin().");

	m_pShaderProgram->Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_bDrawing = true;
}

GLvoid MLineRenderer::Flush()
{
	glBindVertexArray(m_hVAO);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertexData.size() * sizeof(dcel::vertex_attr), &m_vVertexData[0], GL_STATIC_DRAW);

	// Set vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(dcel::vertex_attr, pos)); // Position
	glVertexAttribBinding(0, 0);

	// Bind vertex buffers
	glBindVertexBuffer(0, m_hVBO, 0, sizeof(dcel::vertex_attr));

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndexData.size() * sizeof(GLuint), &m_vIndexData[0], GL_STATIC_DRAW);

	// Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_vIndirectData.size() * sizeof(DrawElementsCommand), &m_vIndirectData[0], GL_STATIC_DRAW);

	// DrawID
	glBindBuffer(GL_ARRAY_BUFFER, m_hIndirectBO);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(1, 1); // only once per instance

	// SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_hSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_vModelMatrices.size() * sizeof(glm::mat4), &m_vModelMatrices[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_hSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	glMultiDrawElementsIndirect(GL_LINES,
		GL_UNSIGNED_INT,
		nullptr,
		m_vIndirectData.size(),
		0);

	glBindVertexArray(0); // Unbind

	m_vVertexData.clear();
	m_vIndirectData.clear();
	m_vIndexData.clear();
	m_vModelMatrices.clear();
	m_vColor.clear();
	m_vDrawId.clear();
	m_nBaseVertex = 0;
	m_nInstanceBase = 0;
	m_bFlush = false;
}

GLvoid MLineRenderer::End()
{
	if (!m_bDrawing)
		throw std::invalid_argument("MShapeRenderer.Begin() must be called before End().");

	Flush();

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Disable shader
	m_pShaderProgram->Disable();

	m_bDrawing = false;
}

GLvoid MLineRenderer::Draw(std::shared_ptr<MMesh>& mesh)
{
	// Store indices
	m_vIndexData.insert(m_vIndexData.end(), mesh->GetEdgeIndices().begin(), mesh->GetEdgeIndices().end());
	m_vVertexData.insert(m_vVertexData.end(), mesh->GetVertices().begin(), mesh->GetVertices().end());

	m_vIndirectData.push_back(DrawElementsCommand{
			mesh->GetIndices().size(),  // Vertex Count
			1,							// Instance Count
			0,							// FirstIndex
			m_nBaseVertex,				// BaseVertex
			m_nInstanceBase				// BaseInstance
		});

	m_vModelMatrices.push_back(mesh->GetModelMatrix());
	m_nBaseVertex += mesh->GetVertices().size();
	m_nInstanceBase++;

	if (m_bFlush) {
		Flush();
	}
}

GLvoid MLineRenderer::InitShaders()
{
	// Create Vertex Shader
	std::shared_ptr<MShader> vertexShader =
		std::make_shared<MShader>(MskShaderType::Vertex);

	// Create Fragment Shader
	std::shared_ptr<MShader> fragmentShader =
		std::make_shared<MShader>(MskShaderType::Fragment);

	// Create shader program
	m_pShaderProgram = std::make_shared<MShaderProgram>();

	// Load and compile shaders
	vertexShader->LoadFromString(lineRenderer_vs);
	vertexShader->Compile();
	fragmentShader->LoadFromString(lineRenderer_fs);
	fragmentShader->Compile();

	// Attach shaders
	m_pShaderProgram->AttachShader(vertexShader);
	m_pShaderProgram->AttachShader(fragmentShader);
	m_pShaderProgram->Link();

	m_pShaderProgram->AddUniform("MVP");
}

GLvoid MLineRenderer::InitBuffers()
{
	glGenVertexArrays(1, &m_hVAO);
	glGenBuffers(1, &m_hSSBO);
	glGenBuffers(1, &m_hVBO);
	glGenBuffers(1, &m_hIBO);
	glGenBuffers(1, &m_hCBO);
	glGenBuffers(1, &m_hIndirectBO);
	glGenBuffers(1, &m_hDrawId);
	glGenBuffers(1, &m_hModelMatrixId);
}
