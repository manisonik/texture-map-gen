#include "stdafx.h"
#include "msk-gl-spritebatch.h"

using namespace Manisonik;

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

MSpriteBatch::MSpriteBatch()
{
	// Set index to 0
	m_nBaseVertex = 0;

	// Set id to default
	m_nCurrentTextureId = -1;

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

	// Add index data
	m_vIndexData.insert(m_vIndexData.begin(), { 0, 1, 2, 2, 3, 0 });
}

MSpriteBatch::~MSpriteBatch()
{
	glDeleteBuffers(1, &m_hVBO);
	glDeleteBuffers(1, &m_hIBO);
	glDeleteBuffers(1, &m_hVAO);
	glDeleteBuffers(1, &m_hIndirectBO);
	glDeleteBuffers(1, &m_hModelMatrixId);
}

void MSpriteBatch::SetProjectionMatrix(glm::mat4& matProj)
{
	m_matProjection = matProj;
}

void MSpriteBatch::Begin()
{
	if (m_bDrawing)
		throw std::invalid_argument("MRenderer.End() must be called before Begin().");
	
	m_pShaderProgram->Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	m_bDrawing = true;
}

void MSpriteBatch::Flush()
{
	m_pShaderProgram->SetUniform("MVP", m_matProjection);

	glBindVertexArray(m_hVAO);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertexData.size() * sizeof(VertexPositionTexture), &m_vVertexData[0], GL_STATIC_DRAW);

	// Set vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 4, GL_FLOAT, false, offsetof(VertexPositionTexture, position)); // Position
	glVertexAttribBinding(0, 0);

	// Bind vertex buffers
	glBindVertexBuffer(0, m_hVBO, 0, sizeof(VertexPositionTexture));

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

	// Render triangles
	glMultiDrawElementsIndirect(GL_TRIANGLES,
		GL_UNSIGNED_INT,
		nullptr,
		m_vIndirectData.size(),
		0);
	
	glBindVertexArray(0); // Unbind

	m_vVertexData.clear();
	m_vIndirectData.clear();
	m_vModelMatrices.clear();
	m_nBaseVertex = 0;
	m_nInstanceBase = 0;
	m_bFlush = false;
}

void MSpriteBatch::End()
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

void MSpriteBatch::Draw(std::shared_ptr<MTexture>& texture, glm::mat4 transformMatrix)
{
	// Create quad
	//m_vVertexData.push_back({ glm::vec2(0.5f, -0.5f), glm::vec2(region->GetU(), region->GetV()) }); // 0
	//m_vVertexData.push_back({ glm::vec2(-0.5f, -0.5f), glm::vec2(region->GetU2(), region->GetV()) }); // 1
	//m_vVertexData.push_back({ glm::vec2(-0.5f, 0.5f), glm::vec2(region->GetU2(), region->GetV2()) }); // 2
	//m_vVertexData.push_back({ glm::vec2(0.5f, 0.5f),glm::vec2(region->GetU(), region->GetV2()) }); // 3

	m_vIndirectData.push_back(DrawElementsCommand{
			m_vIndexData.size(),  // Vertex Count
			1,							// Instance Count
			0,							// FirstIndex
			m_nBaseVertex,				// BaseVertex
			m_nInstanceBase				// BaseInstance
		});

	m_vModelMatrices.push_back(transformMatrix);
	m_nBaseVertex += 4;
	m_nInstanceBase++;

	if (m_bFlush) {
		Flush();
	}
}

void MSpriteBatch::Draw(std::shared_ptr<MTextureRegion>& region, float x, float y, float width, float height)
{
	// Flush if we are using a different texture
	if (m_nCurrentTextureId != -1 && m_nCurrentTextureId != region->GetTexture()->GetTextureId()) {
		Flush();
	}

	// Create quad
	m_vVertexData.push_back({ glm::vec2(x + width, y), glm::vec2(region->GetU2(), region->GetV()) }); // 0
	m_vVertexData.push_back({ glm::vec2(x, y), glm::vec2(region->GetU(), region->GetV()) }); // 1
	m_vVertexData.push_back({ glm::vec2(x, y + height), glm::vec2(region->GetU(), region->GetV2()) }); // 2
	m_vVertexData.push_back({ glm::vec2(x + width, y + height),glm::vec2(region->GetU2(), region->GetV2()) }); // 3

	m_vIndirectData.push_back(DrawElementsCommand{
			m_vIndexData.size(),	// Vertex Count
			1,						// Instance Count
			0,						// FirstIndex
			m_nBaseVertex,			// BaseVertex
			m_nInstanceBase			// BaseInstance
		});

	m_nBaseVertex += 4;
	m_nInstanceBase++;

	// Bind texture
	region->GetTexture()->Bind();

	// Set current texture id
	m_nCurrentTextureId = region->GetTexture()->GetTextureId();
}

void MSpriteBatch::InitShaders()
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

	// Load and compile shaders
	vertexShader->LoadFromString(vs_spritebatch);
	vertexShader->Compile();
	fragmentShader->LoadFromString(fs_spritebatch);
	fragmentShader->Compile();

	// Attach shaders
	m_pShaderProgram->AttachShader(vertexShader);
	m_pShaderProgram->AttachShader(fragmentShader);
	m_pShaderProgram->Link();

	m_pShaderProgram->AddUniform("MVP");
}

void MSpriteBatch::InitBuffers()
{
	glGenVertexArrays(1, &m_hVAO);
	glGenBuffers(1, &m_hSSBO);
	glGenBuffers(1, &m_hVBO);
	glGenBuffers(1, &m_hIBO);
	glGenBuffers(1, &m_hIndirectBO);
	glGenBuffers(1, &m_hDrawId);
	glGenBuffers(1, &m_hModelMatrixId);
}
