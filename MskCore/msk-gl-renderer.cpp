#include "stdafx.h"
#include "msk-gl-renderer.h"

using namespace Manisonik;

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

MRenderer::MRenderer()
{
	m_nBaseVertex = 0;
	m_nBaseFaceIndex = 0;
	m_nInstanceBase = 0;
	m_nInstanceEdgeBase = 0;
	m_nBaseIndirectFaceIndex = 0;
	m_nBaseIndirectEdgeIndex = 0;
	m_nBaseEdgeIndex = 0;
	m_nBaseModelMatrix = 0;
	m_bFlush = false;
	m_bDrawing = false;

	// Setup buffer size
	m_nNumOfVertices = 10000;
	m_nNumOfIndices = 10000;
	m_nNumOfObjects = 10000;
	m_nNumOfEdgeIndices = 10000;

	// Initalize the shaders
	InitShaders();

	// Generate Vertex Attribute Object
	InitBuffers();

	// Fill Buffers
	FillBuffers();
}

MRenderer::~MRenderer()
{
	glDeleteBuffers(1, &m_hVBO);
	glDeleteBuffers(1, &m_hIBO);
	glDeleteBuffers(1, &m_hCBO);
	glDeleteBuffers(1, &m_hVAO);
	glDeleteBuffers(1, &m_hDrawId);
	glDeleteBuffers(1, &m_hIndirectBO);
	glDeleteBuffers(1, &m_hModelMatrixId);
}

GLvoid MRenderer::SetProjectionMatrix(glm::mat4& matProj)
{
	m_matProjection = matProj;
}

GLvoid MRenderer::AddMesh(std::shared_ptr<MMesh>& mesh)
{
	// Generate vertices from mesh
	std::vector<dcel::vertex_attr> vertices = mesh->m_pManifold->generate_vertices();

	// Generate face indices from mesh
	std::vector<GLuint> faceIndices = mesh->m_pManifold->generate_face_indices();

	// Generate vertex indices from mesh
	std::vector<GLuint> edgeIndices = mesh->m_pManifold->generate_edge_indices();

	// Get the model matrix
	glm::mat4 modelMatrix = mesh->GetModelMatrix();

	DrawElementsCommand faceIndirect = DrawElementsCommand{
			faceIndices.size(), // Vertex Count
			1,							// Instance Count
			m_nBaseFaceIndex,			// FirstIndex
			m_nBaseVertex,				// BaseVertex
			m_nInstanceBase				// BaseInstance
		};

	DrawElementsCommand edgeIndirect = DrawElementsCommand{
			edgeIndices.size(), // Vertex Count
			1,							   // Instance Count
			m_nBaseEdgeIndex,			   // FirstIndex
			m_nBaseVertex,				   // BaseVertex
			m_nInstanceEdgeBase			   // BaseInstance
		};

	glBindVertexArray(m_hVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferSubData(GL_ARRAY_BUFFER, m_nBaseVertex * sizeof(dcel::vertex_attr), vertices.size() * sizeof(dcel::vertex_attr), &vertices[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_hSSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_nBaseModelMatrix * sizeof(glm::mat4), sizeof(glm::mat4), &modelMatrix);

	if (faceIndices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_nBaseFaceIndex * sizeof(GLuint), faceIndices.size() * sizeof(GLuint), &faceIndices[0]);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, m_nBaseIndirectFaceIndex * sizeof(DrawElementsCommand), sizeof(DrawElementsCommand), &faceIndirect);
		m_nBaseFaceIndex += faceIndices.size();
		m_nBaseIndirectFaceIndex++;
	}

	if (edgeIndices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hEdgeIBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_nBaseEdgeIndex * sizeof(GLuint), edgeIndices.size() * sizeof(GLuint), &edgeIndices[0]);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectEdgeBO);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, m_nBaseIndirectEdgeIndex * sizeof(DrawElementsCommand), sizeof(DrawElementsCommand), &edgeIndirect);
		m_nBaseEdgeIndex += edgeIndices.size();
		m_nBaseIndirectEdgeIndex++;
	}

	glBindVertexArray(0); // Unbind
	m_nBaseVertex += vertices.size();
	m_nBaseModelMatrix++;
	m_nInstanceBase++;
	m_nInstanceEdgeBase++;
}

GLvoid MRenderer::Draw()
{
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(m_hVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_hSSBO);

	// ==========================================
	// Render triangles
	// ==========================================
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);

	m_pShaderProgram->Use();
	m_pShaderProgram->SetUniform("MVP", m_matProjection);

	glMultiDrawElementsIndirect(GL_TRIANGLES,
		GL_UNSIGNED_INT,
		nullptr,
		m_nBaseIndirectFaceIndex,
		0);

	// ==========================================
	//  Render edges
	// ==========================================
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hEdgeIBO);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectEdgeBO);

	m_pShaderProgramPoints->Use();
	m_pShaderProgramPoints->SetUniform("MVP", m_matProjection);

	glMultiDrawElementsIndirect(GL_LINES,
		GL_UNSIGNED_INT,
		nullptr,
		m_nBaseIndirectEdgeIndex,
		0);
	// ==========================================
	//  Render points
	// ==========================================
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);
	
	glMultiDrawElementsIndirect(GL_POINTS,
		GL_UNSIGNED_INT,
		nullptr,
		m_nBaseIndirectFaceIndex,
		0);
	glClear(GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(0); // Unbind

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Disable shader
	m_pShaderProgram->Disable();
}

GLvoid MRenderer::Begin()
{
	if (m_bDrawing)
		throw std::invalid_argument("MRenderer.End() must be called before Begin().");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_bDrawing = true;
}

GLvoid MRenderer::Flush()
{
	glBindVertexArray(m_hVAO);

	// Return if no vertices present
	if (m_vVertexData.size() == 0)
		return;

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertexData.size() * sizeof(dcel::vertex_attr), &m_vVertexData[0], GL_STATIC_DRAW);

	// Set vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(dcel::vertex_attr, pos)); // Position
	glVertexAttribBinding(0, 0);

	// Bind vertex buffers
	glBindVertexBuffer(0, m_hVBO, 0, sizeof(dcel::vertex_attr));

	// SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_hSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_vModelMatrices.size() * sizeof(glm::mat4), &m_vModelMatrices[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_hSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	// ==================================================
	// Render Triangles
	// ==================================================

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

	m_pShaderProgram->Use();
	m_pShaderProgram->SetUniform("MVP", m_matProjection);

	// Render triangles
	glMultiDrawElementsIndirect(GL_TRIANGLES,
		GL_UNSIGNED_INT,
		nullptr,
		m_vIndirectData.size(),
		0);

	m_pShaderProgramPoints->Use();
	m_pShaderProgramPoints->SetUniform("MVP", m_matProjection);

	// ==================================================
	// Render lines
	// ==================================================

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hEdgeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndexEdgeData.size() * sizeof(uint32_t), &m_vIndexEdgeData[0], GL_STATIC_DRAW);

	// Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectEdgeBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_vIndirectEdgeData.size() * sizeof(DrawElementsCommand), &m_vIndirectEdgeData[0], GL_STATIC_DRAW);

	// DrawID
	glBindBuffer(GL_ARRAY_BUFFER, m_hIndirectEdgeBO);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(1, 1); // only once per instance

	// Render edges
	glMultiDrawElementsIndirect(GL_LINES,
		GL_UNSIGNED_INT,
		nullptr,
		m_vIndirectEdgeData.size(),
		0);

	// ==================================================
	// Render points
	// ==================================================
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_hIndirectBO);

	// Render points
	glMultiDrawElementsIndirect(GL_POINTS,
		GL_UNSIGNED_INT,
		nullptr,
		m_vIndirectData.size(),
		0);

	glBindVertexArray(0); // Unbind

	m_vVertexData.clear();
	m_vIndirectData.clear();
	m_vIndirectEdgeData.clear();
	m_vIndexEdgeData.clear();
	m_vIndexData.clear();
	m_vModelMatrices.clear();
	m_vColor.clear();
	m_vDrawId.clear();
	m_nBaseVertex = 0;
	m_nBaseFaceIndex = 0;
	m_nInstanceBase = 0;
	m_bFlush = false;
}

GLvoid MRenderer::End()
{
	if (!m_bDrawing)
		throw std::invalid_argument("MRenderer.Begin() must be called before End().");

	Flush();

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Disable shader
	m_pShaderProgram->Disable();

	m_bDrawing = false;
}

GLvoid MRenderer::Draw(std::shared_ptr<MMesh>& mesh)
{
	// Store indices
	m_vIndexData.insert(m_vIndexData.end(), mesh->GetIndices().begin(), mesh->GetIndices().end());
	m_vIndexEdgeData.insert(m_vIndexEdgeData.end(), mesh->GetEdgeIndices().begin(), mesh->GetEdgeIndices().end());
	m_vVertexData.insert(m_vVertexData.end(), mesh->GetVertices().begin(), mesh->GetVertices().end());

	m_vIndirectData.push_back(DrawElementsCommand{
			 mesh->GetIndices().size(),  // Vertex Count
			1,				 // Instance Count
			0,				 // FirstIndex
			m_nBaseVertex,	 // BaseVertex
			m_nInstanceBase	 // BaseInstance
		});

	m_vIndirectEdgeData.push_back(DrawElementsCommand{
			mesh->GetEdgeIndices().size(),  // Vertex Count
			1,				 // Instance Count
			m_nBaseFaceIndex,	 // FirstIndex
			m_nBaseVertex,   // BaseVertex
			m_nInstanceBase	 // BaseInstance
		});

	m_vModelMatrices.push_back(mesh->GetModelMatrix());
	m_nBaseVertex += mesh->GetVertices().size();
	m_nBaseFaceIndex += mesh->GetEdgeIndices().size();
	m_nInstanceBase++;

	if (m_bFlush) {
		Flush();
	}
}

GLvoid MRenderer::FillBuffers()
{
	glBindVertexArray(m_hVAO);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_nNumOfVertices * sizeof(dcel::vertex_attr), nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(dcel::vertex_attr, pos)); // Position
	glVertexAttribBinding(0, 0);

	// Bind vertex buffers
	glBindVertexBuffer(0, m_hVBO, 0, sizeof(dcel::vertex_attr));

	// Mesh SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_hSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_nNumOfObjects * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_hSSBO);

	// Face Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nNumOfIndices* sizeof(GLuint), nullptr, GL_STATIC_DRAW);

	// Face Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_nNumOfObjects * sizeof(DrawElementsCommand), nullptr, GL_STATIC_DRAW);

	// Face DrawID
	glBindBuffer(GL_ARRAY_BUFFER, m_hIndirectBO);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT,  sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(1, 1); // only once per instance

	// Edge Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hEdgeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nNumOfEdgeIndices * sizeof(uint32_t), nullptr, GL_STATIC_DRAW);

	// Edge Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectEdgeBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_nNumOfObjects * sizeof(DrawElementsCommand), nullptr, GL_STATIC_DRAW);

	// Edge DrawID
	glBindBuffer(GL_ARRAY_BUFFER, m_hIndirectEdgeBO);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
	glVertexAttribDivisor(1, 1); // only once per instance

	glBindVertexArray(0); // Unbind
}

GLvoid MRenderer::InitShaders()
{
	// Create Vertex Shader
	std::shared_ptr<MShader> vertexShader =
		std::make_shared<MShader>(MskShaderType::Vertex);

	// Create Fragment Shader
	std::shared_ptr<MShader> fragmentShader =
		std::make_shared<MShader>(MskShaderType::Fragment);

	// Create Fragment Shader for points
	std::shared_ptr<MShader> fragmentShaderPoints =
		std::make_shared<MShader>(MskShaderType::Fragment);

	// Create shader program
	m_pShaderProgram = std::make_shared<MShaderProgram>();
	m_pShaderProgramPoints = std::make_shared<MShaderProgram>();

	// Load and compile shaders
	vertexShader->LoadFromString(vs);
	vertexShader->Compile();
	fragmentShader->LoadFromString(fs);
	fragmentShader->Compile();
	fragmentShaderPoints->LoadFromString(fs_points);
	fragmentShaderPoints->Compile();

	// Attach shaders
	m_pShaderProgram->AttachShader(vertexShader);
	m_pShaderProgram->AttachShader(fragmentShader);
	m_pShaderProgram->Link();

	m_pShaderProgramPoints->AttachShader(vertexShader);
	m_pShaderProgramPoints->AttachShader(fragmentShaderPoints);
	m_pShaderProgramPoints->Link();
	
	m_pShaderProgram->AddUniform("MVP");
	m_pShaderProgramPoints->AddUniform("MVP");
}

GLvoid MRenderer::InitBuffers()
{
	glGenVertexArrays(1, &m_hVAO);
	glGenBuffers(1, &m_hSSBO);
	glGenBuffers(1, &m_hVBO);
	glGenBuffers(1, &m_hIBO);
	glGenBuffers(1, &m_hEdgeIBO);
	glGenBuffers(1, &m_hCBO);
	glGenBuffers(1, &m_hIndirectBO);
	glGenBuffers(1, &m_hIndirectEdgeBO);
	glGenBuffers(1, &m_hDrawId);
	glGenBuffers(1, &m_hModelMatrixId);
}
