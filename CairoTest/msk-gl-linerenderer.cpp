#include "stdafx.h"
#include "msk-gl-linerenderer.h"
#include "msk-gl-framerenderer.h"

const char* lineRenderer_vs = R"glsl(
	#version 440 core

		// Input vertex data, different for all executions of this shader.
	layout(location = 0) in vec3 vertexPosition_modelspace;
	layout(location = 1) in vec4 vertexColor;
	layout(location = 2) in uint drawId;
	layout(location = 3) in mat4 instanceMatrix;
  
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

const char* lineRenderer_fs = R"glsl(
	#version 440 core
	// Input color from vertex shader
	in vec4 color;

	// Output
	out vec4 outColor;

	void main()
	{
		outColor = color;
	} 
)glsl";

MLineRenderer::MLineRenderer(MskApp* app)
{
	// Set index to 0
	m_nBaseVertex = 0;

	// Set flush to false
	m_bFlush = false;

	// Initalize the shaders
	InitShaders();

	// Generate Vertex Attribute Object
	glGenVertexArrays(1, &m_hVAO);
	glGenBuffers(1, &m_hVBO);
	glGenBuffers(1, &m_hCBO);
	glGenBuffers(1, &m_hIBO);
	glGenBuffers(1, &m_hIndirectBO);
	glGenBuffers(1, &m_hDrawId);
	glGenBuffers(1, &m_hModelMatrixId);
}

MLineRenderer::~MLineRenderer()
{
	glDeleteBuffers(1, &m_hVBO);
	glDeleteBuffers(1, &m_hIBO);
	glDeleteBuffers(1, &m_hCBO);
	glDeleteBuffers(1, &m_hVAO);
	glDeleteBuffers(1, &m_hDrawId);
	glDeleteBuffers(1, &m_hModelMatrixId);
	glDeleteBuffers(1, &m_hIndirectBO);
}

GLvoid MLineRenderer::InitShaders()
{
	MskFactory::CreateShader(MskShaderType::Vertex, m_pVertexShader);
	MskFactory::CreateShader(MskShaderType::Fragment, m_pFragmentShader);
	MskFactory::CreateShaderProgram(m_pShaderProgram);

	m_pVertexShader->LoadFromString(lineRenderer_vs);
	m_pVertexShader->Compile();
	m_pFragmentShader->LoadFromString(lineRenderer_fs);
	m_pFragmentShader->Compile();

	m_pShaderProgram->AttachShader(m_pVertexShader);
	m_pShaderProgram->AttachShader(m_pFragmentShader);
	m_pShaderProgram->Link();

	// Set MVP Matrix
	m_hMVPMatrixId = m_pShaderProgram->AddUniform("MVP");
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

GLvoid MLineRenderer::AntiAliasing(glm::vec3 p1, glm::vec3 p2, GLfloat radius)
{
	GLfloat w = 1;

	// Project line end-points to screen coordinates
	glm::vec3 s0 = glm::project(p1, mskCamera->GetModelMatrix(), mskCamera->GetProjectionMatrix(), glm::vec4(0, 0, 800, 600));
	glm::vec3 s1 = glm::project(p2, mskCamera->GetModelMatrix(), mskCamera->GetProjectionMatrix(), glm::vec4(0, 0, 800, 600));

	GLfloat k = 2 / ((2 * radius + w) * glm::distance(p1, p2));

	// Compute the coefficients of the four linear edge functions
	glm::vec3 e0 = glm::vec3(k * (s0.y - s1.y), k * (s1.x - s0.x), 1 + k * (s0.x * s1.y - s1.x * s0.y));
	glm::vec3 e1 = glm::vec3(k * (s1.x - s0.x), k * (s1.y - s0.y), 1 + k * ((s0.x - s0.x) + (s0.y * s0.y) - (s0.x * s1.x) - (s0.y * s1.y)));
	glm::vec3 e2 = glm::vec3(k * (s1.y - s0.y), k * (s0.x - s1.x), 1 + k * (s1.x * s0.y - s0.x * s1.y));
	glm::vec3 e3 = glm::vec3(k * (s0.x - s1.x), k * (s0.y - s1.y), 1 + k * ((s1.x - s1.x) + (s1.y * s1.y) - (s0.x * s1.x) - (s0.y * s1.y)));

	// The four sets of coefficients are passed to a pixel shader as uniform
	m_pShaderProgram->SetUniform("edge0", e0);
	m_pShaderProgram->SetUniform("edge1", e1);
	m_pShaderProgram->SetUniform("edge2", e2);
	m_pShaderProgram->SetUniform("edge3", e3);

	// Our pixel shader won't do anything useful without any fragments
	// Therefore, we must perform conservative rasterization and make sure that 
	// all the fragments that lie within a distance of w/2 + R are generated.
	glLineWidth(ceil((2.0f * radius + w) * sqrt(2.0f)));
}

GLvoid MLineRenderer::SetProjectionMatrix(glm::mat4& matProj)
{
	m_pShaderProgram->SetUniform("MVP", matProj);
}

GLvoid MLineRenderer::Draw(std::shared_ptr<MskMesh>& mesh)
{
	// Store indices
	//m_vIndexData.insert(m_vIndexData.end(), mesh->GetIndices().begin(), mesh->GetIndices().end());
	m_vVertexData.insert(m_vVertexData.end(), mesh->GetVertices().begin(), mesh->GetVertices().end());

	GLuint indexCount = 0;
	for (auto face = mesh->GetFaces().begin(); face != mesh->GetFaces().end(); ++face)
	{
		if (face->indices.size() == 2)
		{
			m_vIndexData.push_back(face->indices[0]);
			m_vIndexData.push_back(face->indices[1]);
			indexCount += 2;
		}
		else if (face->indices.size() == 6)
		{
			m_vIndexData.push_back(face->indices[0]);
			m_vIndexData.push_back(face->indices[1]);
			m_vIndexData.push_back(face->indices[0]);
			m_vIndexData.push_back(face->indices[2]);
			m_vIndexData.push_back(face->indices[1]);
			m_vIndexData.push_back(face->indices[2]);
			m_vIndexData.push_back(face->indices[3]);
			m_vIndexData.push_back(face->indices[4]);
			m_vIndexData.push_back(face->indices[3]);
			m_vIndexData.push_back(face->indices[5]);
			m_vIndexData.push_back(face->indices[4]);
			m_vIndexData.push_back(face->indices[5]);
			indexCount += 12;
		}
	}

	m_vIndirectData.push_back( DrawElementsCommand {
				indexCount,			// Vertex Count
				1,					// Instance Count
				m_nFirstIndex,		// FirstIndex
				m_nBaseVertex,		// BaseVertex
				m_nInstanceBase		// BaseInstance
		});
	
	m_vColor.push_back(mesh->GetColor());
	m_vModelMatrices.push_back(mesh->GetModelMatrix());
	m_nBaseVertex += mesh->GetVertices().size();
	m_nFirstIndex += indexCount;
	m_nInstanceBase++;
}

GLvoid MLineRenderer::Flush()
{
	if (m_vIndirectData.size() == 0 || m_vVertexData.size() == 0)
		return;

	glBindVertexArray(m_hVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertexData.size() * sizeof(glm::vec3), &m_vVertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);  // Position

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_vIndirectData.size() * sizeof(DrawElementsCommand), &m_vIndirectData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndexData.size() * sizeof(GLushort), &m_vIndexData[0], GL_STATIC_DRAW);

	//feed the instance id to the shader.
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

	glMultiDrawElementsIndirect(GL_LINES,
		GL_UNSIGNED_SHORT,
		nullptr,
		m_vIndirectData.size(),
		0);

	glBindVertexArray(0); // Unbind

	m_vVertexData.clear();
	m_vIndirectData.clear();
	m_vIndexData.clear();
	m_vColor.clear();
	m_vModelMatrices.clear();
	m_nBaseVertex = 0;
	m_nFirstIndex = 0;
	m_nInstanceBase = 0;
	m_bFlush = false;
}
