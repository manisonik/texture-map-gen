#include "stdafx.h"
#include "msk-gl-shaperenderer.h"

const char* vs = R"glsl(
	#version 440 core

	// Input vertex data, different for all executions of this shader.
	layout(location = 0) in vec3 vertexPosition_modelspace;
	layout(location = 1) in vec4 vertexColor;
	layout(location = 2) in uint drawId;
	layout(location = 3) in mat4 instanceMatrix;

	//layout(std430, binding = 0) buffer modelMatrices
	//{
		//mat4 model[];
	//};
  
	// Values that stay constant for the whole mesh.
	uniform mat4 MVP;

	// Output data
	out vec4 color;

	void main()
	{
		gl_Position = MVP * instanceMatrix * vec4(vertexPosition_modelspace, 1.0f);

		// Pass on the fragment shader
		color = vertexColor;
	}
)glsl";

const char* fs = R"glsl(
	#version 440 core

	// Input color from vertex shader

	// Output
	in vec4 color;
	out vec4 outColor;

	void main()
	{
		outColor = color;
	} 
)glsl";

MShapeRenderer::MShapeRenderer()
{
	// Set to default
	m_nPrimitiveType = GL_TRIANGLES;

	// Set index to 0
	m_nBaseVertex = 0;

	// Set flush to false
	m_bFlush = false;

	// Initalize the shaders
	InitShaders();

	// Generate Vertex Attribute Object
	glGenVertexArrays(1, &m_hVAO);
	glGenBuffers(1, &m_hVBO);
	glGenBuffers(1, &m_hIBO);
	glGenBuffers(1, &m_hCBO);
	glGenBuffers(1, &m_hIndirectBO);
	glGenBuffers(1, &m_hDrawId);
	glGenBuffers(1, &m_hModelMatrixId);
}

MShapeRenderer::~MShapeRenderer()
{
	glDeleteBuffers(1, &m_hVBO);
	glDeleteBuffers(1, &m_hIBO);
	glDeleteBuffers(1, &m_hCBO);
	glDeleteBuffers(1, &m_hVAO);
	glDeleteBuffers(1, &m_hDrawId);
	glDeleteBuffers(1, &m_hIndirectBO);
	glDeleteBuffers(1, &m_hModelMatrixId);
}

GLvoid MShapeRenderer::InitShaders() {
	MskFactory::CreateShader(MskShaderType::Vertex, m_pVertexShader);
	MskFactory::CreateShader(MskShaderType::Fragment, m_pFragmentShader);
	MskFactory::CreateShaderProgram(m_pShaderProgram);

	m_pVertexShader->LoadFromString(vs);
	m_pVertexShader->Compile();
	m_pFragmentShader->LoadFromString(fs);
	m_pFragmentShader->Compile();

	m_pShaderProgram->AttachShader(m_pVertexShader);
	m_pShaderProgram->AttachShader(m_pFragmentShader);
	m_pShaderProgram->Link();

	// Set MVP Matrix
	m_hMVPMatrixId = m_pShaderProgram->AddUniform("MVP");
}

GLvoid MShapeRenderer::SetProjectionMatrix(glm::mat4 & matProj)
{
	m_pShaderProgram->SetUniform("MVP", matProj);
}

GLvoid MShapeRenderer::Begin()
{
	if (m_bDrawing)
		throw std::invalid_argument("MShapeRenderer.End() must be called before Begin().");

	m_pShaderProgram->Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_bDrawing = true;
}

GLvoid MShapeRenderer::End()
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

GLvoid MShapeRenderer::Draw(std::shared_ptr<MskMesh>& mesh)
{
	// Store indices
	m_vIndexData.insert(m_vIndexData.end(), mesh->GetIndices().begin(), mesh->GetIndices().end());
	m_vVertexData.insert(m_vVertexData.end(), mesh->GetVertices().begin(), mesh->GetVertices().end());

	m_vIndirectData.push_back( DrawElementsCommand {
			mesh->GetIndices().size(), // Vertex Count
			1,					 // Instance Count
			0,					 // FirstIndex
			m_nBaseVertex,		 // BaseVertex
			m_nInstanceBase		 // BaseInstance
	});

	m_vColor.push_back(mesh->GetColor());
	m_vModelMatrices.push_back(mesh->GetModelMatrix());
	m_nBaseVertex += mesh->GetVertices().size();
	m_nInstanceBase++;

	if (m_bFlush) {
		Flush();
	}
}

GLvoid MShapeRenderer::Flush()
{
	if (m_vIndirectData.size() <= 0 || m_vVertexData.size() <= 0)
		return;

	glBindVertexArray(m_hVAO);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertexData.size() * sizeof(glm::vec3), &m_vVertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);  // Position

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndexData.size() * sizeof(GLushort), &m_vIndexData[0], GL_STATIC_DRAW);

	// Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_vIndirectData.size() * sizeof(DrawElementsCommand), &m_vIndirectData[0], GL_STATIC_DRAW);
	
	// Indirect Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hIndirectBO);
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(2, 1); // only once per instance

	// Color Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hCBO);
	glBufferData(GL_ARRAY_BUFFER, m_vColor.size() * sizeof(glm::vec4), &m_vColor[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0); // Color
	glVertexAttribDivisor(1, 1); // only once per instance

	glBindBuffer(GL_ARRAY_BUFFER, m_hModelMatrixId);
	glBufferData(GL_ARRAY_BUFFER, m_vModelMatrices.size() * sizeof(glm::mat4), &m_vModelMatrices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glMultiDrawElementsIndirect(GL_TRIANGLES,
		GL_UNSIGNED_SHORT,
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