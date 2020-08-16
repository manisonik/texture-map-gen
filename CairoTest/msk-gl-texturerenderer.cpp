#include "stdafx.h"
#include "msk-gl-texturerenderer.h"
#include "msk-gl-mesh.h"

MTextureRenderer::MTextureRenderer() :
	m_nVertexStride(0),
	m_nBaseVertex(0),
	m_bFlush(false)
{
	// Load the vertex and fragment shaders
	InitShaders();

	// Generate Vertex Attribute Object
	glGenVertexArrays(1, &m_hVAO);
	glGenBuffers(1, &m_hVBO);
	glGenBuffers(1, &m_hBNBO);
	glGenBuffers(1, &m_hTNBO);
	glGenBuffers(1, &m_hTBO);
	glGenBuffers(1, &m_hNBO);
	glGenBuffers(1, &m_hCBO);
	glGenBuffers(1, &m_hIBO);
	glGenBuffers(1, &m_hIndirectBO);
	glGenBuffers(1, &m_hDrawId);
	glGenBuffers(1, &m_hModelMatrixId);
}

MTextureRenderer::~MTextureRenderer()
{
	glDeleteBuffers(1, &m_hVBO);
	glDeleteBuffers(1, &m_hTBO);
	glDeleteBuffers(1, &m_hBNBO);
	glDeleteBuffers(1, &m_hTNBO);
	glDeleteBuffers(1, &m_hNBO);
	glDeleteBuffers(1, &m_hCBO);
	glDeleteBuffers(1, &m_hIBO);
	glDeleteBuffers(1, &m_hVAO);
	glDeleteBuffers(1, &m_hIndirectBO);
	glDeleteBuffers(1, &m_hDrawId);
	glDeleteBuffers(1, &m_hModelMatrixId);

	m_pBasicTextureVS.reset();
	m_pBasicTextureFS.reset();
	m_pBasicTextureSP.reset();
}

GLvoid MTextureRenderer::InitShaders() {
	MskFactory::CreateShader(MskShaderType::Vertex, m_pBasicTextureVS);
	MskFactory::CreateShader(MskShaderType::Fragment, m_pBasicTextureFS);
	MskFactory::CreateShaderProgram(m_pBasicTextureSP);

	m_pBasicTextureVS->LoadFromFile(".\\basic_texture.glvs");
	m_pBasicTextureVS->Compile();
	m_pBasicTextureFS->LoadFromFile(".\\basic_texture.glfs");
	m_pBasicTextureFS->Compile();

	m_pBasicTextureSP->AttachShader(m_pBasicTextureVS);
	m_pBasicTextureSP->AttachShader(m_pBasicTextureFS);
	m_pBasicTextureSP->Link();

	// Add MVP matrix
	m_pBasicTextureSP->AddUniform("MVP");

	MskFactory::CreateShader(MskShaderType::Vertex, m_pMaterialTextureVS);
	MskFactory::CreateShader(MskShaderType::Fragment, m_pMaterialTextureFS);
	MskFactory::CreateShaderProgram(m_pMaterialTextureSP);

	m_pMaterialTextureVS->LoadFromFile(".\\material_texture.glvs");
	m_pMaterialTextureVS->Compile();
	m_pMaterialTextureFS->LoadFromFile(".\\material_texture.glfs");
	m_pMaterialTextureFS->Compile();

	m_pMaterialTextureSP->AttachShader(m_pMaterialTextureVS);
	m_pMaterialTextureSP->AttachShader(m_pMaterialTextureFS);
	m_pMaterialTextureSP->Link();

	// Add MVP matrix
	m_pMaterialTextureSP->AddUniform("MVP");
	m_pMaterialTextureSP->AddUniform("viewPos");
	m_pMaterialTextureSP->AddUniform("viewMatrix");	
	m_pMaterialTextureSP->AddUniform("modelMatrix");
	m_pMaterialTextureSP->AddUniform("light.ambient");
	m_pMaterialTextureSP->AddUniform("light.position");
	m_pMaterialTextureSP->AddUniform("light.diffuse");
	m_pMaterialTextureSP->AddUniform("light.specular");
	m_pMaterialTextureSP->AddUniform("material.ambient");
	m_pMaterialTextureSP->AddUniform("material.diffuse");
	m_pMaterialTextureSP->AddUniform("material.specular");
	m_pMaterialTextureSP->AddUniform("material.shininess");

	// POM
	MskFactory::CreateShader(MskShaderType::Vertex, m_pPOMTextureVS);
	MskFactory::CreateShader(MskShaderType::Fragment, m_pPOMTextureFS);
	MskFactory::CreateShaderProgram(m_pPOMTextureSP);

	m_pPOMTextureVS->LoadFromFile(".\\pom_texture.glvs");
	m_pPOMTextureVS->Compile();
	m_pPOMTextureFS->LoadFromFile(".\\pom_texture.glfs");
	m_pPOMTextureFS->Compile();

	m_pPOMTextureSP->AttachShader(m_pPOMTextureVS);
	m_pPOMTextureSP->AttachShader(m_pPOMTextureFS);
	m_pPOMTextureSP->Link();

	// Add MVP matrix
	m_pPOMTextureSP->AddUniform("MVP");
	m_pPOMTextureSP->AddUniform("viewPos");
	m_pPOMTextureSP->AddUniform("heightScale");
	m_pPOMTextureSP->AddUniform("lightPos");
	m_pPOMTextureSP->AddUniform("viewMatrix");
	m_pPOMTextureSP->AddUniform("modelMatrix");
	m_pPOMTextureSP->AddUniform("light.ambient");
	m_pPOMTextureSP->AddUniform("light.position");
	m_pPOMTextureSP->AddUniform("light.diffuse");
	m_pPOMTextureSP->AddUniform("light.specular");
	m_pPOMTextureSP->AddUniform("material.ambient");
	m_pPOMTextureSP->AddUniform("material.diffuse");
	m_pPOMTextureSP->AddUniform("material.specular");
	m_pPOMTextureSP->AddUniform("material.shininess");
}

GLvoid MTextureRenderer::CalculateNormals(
	std::vector<GLshort>& indices,
	std::vector<glm::vec4>& pos,
	std::vector<glm::vec3>& normals
)
{
	if (indices.size() < 3)
		return;

	for (size_t i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 normal = glm::triangleNormal(
			glm::vec3(pos[indices[i]]),
			glm::vec3(pos[indices[i + 1]]),
			glm::vec3(pos[indices[i + 2]])
		);

		normals.insert(normals.end(), 3, normal);
	}
}

GLvoid MTextureRenderer::CalculateTangentBasis(
	std::vector<GLshort>& indices,
	std::vector<glm::vec4>& pos,
	std::vector<glm::vec3>& normals,
	std::vector<glm::vec2>& uv,
	std::vector<glm::vec3>& tangents,
	std::vector<glm::vec3>& bitangents
)
{
	if (indices.size() < 3)
		return;

	for (size_t i = 0; i < indices.size(); i += 3) {
		glm::vec3& v0 = glm::vec3(pos[indices[i]]);
		glm::vec3& v1 = glm::vec3(pos[indices[i + 1]]);
		glm::vec3& v2 = glm::vec3(pos[indices[i + 2]]);

		glm::vec2& uv0 = uv[indices[i]];
		glm::vec2& uv1 = uv[indices[i + 1]];
		glm::vec2& uv2 = uv[indices[i + 2]];

		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		tangents.insert(tangents.end(), 3, tangent);
		bitangents.insert(bitangents.end(), 3, tangent);
	}
}

GLvoid MTextureRenderer::SetCamera(std::shared_ptr<MskCamera>& camera)
{
	if (m_bLighting) {
		m_pMaterialTextureSP->SetUniform("MVP", camera->GetModelViewProjectionMatrix());
		m_pMaterialTextureSP->SetUniform("viewPos", camera->GetPosition());
		m_pMaterialTextureSP->SetUniform("modelMatrix", camera->GetModelMatrix());
		m_pMaterialTextureSP->SetUniform("viewMatrix", camera->GetViewMatrix());
		
	} 
	else 
	{
		m_pBasicTextureSP->SetUniform("MVP", camera->GetModelViewProjectionMatrix());
	}
}

GLvoid MTextureRenderer::SetCamera(glm::mat4& matProj)
{
	if (m_bLighting) {
		m_pMaterialTextureSP->SetUniform("MVP", matProj);
	} else {
		m_pBasicTextureSP->SetUniform("MVP", matProj);
	}
}

GLvoid MTextureRenderer::EnableLighting(GLboolean enable)
{
	m_bLighting = enable;
}

GLvoid MTextureRenderer::EnablePOM(GLboolean enable)
{
	m_bPOM = enable;
}

GLvoid MTextureRenderer::Draw(std::shared_ptr<MskMesh>& mesh)
{
	// Store indices
	m_vIndexData.insert(m_vIndexData.end(), mesh->GetIndices().begin(), mesh->GetIndices().end());
	m_vVertexData.insert(m_vVertexData.end(), mesh->GetVertices().begin(), mesh->GetVertices().end());
	m_vNormalData.insert(m_vNormalData.end(), mesh->GetNormals().begin(), mesh->GetNormals().end());
	m_vTangentData.insert(m_vTangentData.end(), mesh->GetTangents().begin(), mesh->GetTangents().end());
	m_vBitangentData.insert(m_vBitangentData.end(), mesh->GetBitangents().begin(), mesh->GetBitangents().end());
	m_vNormalData.insert(m_vNormalData.end(), mesh->GetNormals().begin(), mesh->GetNormals().end());
	m_vTexCoords.insert(m_vTexCoords.end(), mesh->GetTextureCoords().begin(), mesh->GetTextureCoords().end());

	m_vIndirectData.insert(m_vIndirectData.begin(), {
		{
			mesh->GetIndices().size(), // Vertex Count
			1,					 // Instance Count
			0,					 // FirstIndex
			m_nBaseVertex,		 // BaseVertex
			m_nInstanceBase		 // BaseInstance
		}
	});

	m_vColor.push_back(mesh->GetColor());
	m_vModelMatrices.push_back(mesh->GetModelMatrix());
	m_vDrawId.push_back(0);
	m_nBaseVertex += mesh->GetVertices().size();
	m_nInstanceBase++;

	if (m_bFlush) {
		Flush();
	}
}

GLvoid MTextureRenderer::Begin()
{
	if (m_bDrawing)
		throw std::invalid_argument("MTextureRenderer.End() must be called before Begin().");

	if (m_bLighting) {
		if (m_bPOM) {
			m_pPOMTextureSP->Use();
			m_pPOMTextureSP->SetUniform("light.ambient", glm::vec3(0.3f, 0.3f, 0.31f));
			m_pPOMTextureSP->SetUniform("heightScale", 0.01f);
			m_pPOMTextureSP->SetUniform("lightPos", glm::vec3(0.0f, 0.0f, 2.0));
			m_pPOMTextureSP->SetUniform("light.position", glm::vec3(1.0f, 1.0f, 1.0f));
			m_pPOMTextureSP->SetUniform("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
			m_pPOMTextureSP->SetUniform("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			m_pPOMTextureSP->SetUniform("material.ambient", 0.5f, 0.5f, 0.5f);
			m_pPOMTextureSP->SetUniform("material.diffuse", 0.5f, 0.5f, 0.5f);
			m_pPOMTextureSP->SetUniform("material.specular", 0.5f, 0.5f, 0.5f);
			m_pPOMTextureSP->SetUniform("material.shininess", 32.0f);
		} else {
			m_pMaterialTextureSP->Use();
			m_pMaterialTextureSP->SetUniform("light.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
			m_pMaterialTextureSP->SetUniform("light.position", glm::vec3(0.0f, 0.0f, 1.0f));
			m_pMaterialTextureSP->SetUniform("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
			m_pMaterialTextureSP->SetUniform("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			m_pMaterialTextureSP->SetUniform("material.ambient", 0.5f, 0.5f, 0.5f);
			m_pMaterialTextureSP->SetUniform("material.diffuse", 0.5f, 0.5f, 0.5f);
			m_pMaterialTextureSP->SetUniform("material.specular", 0.8f, 0.8f, 0.8f);
			m_pMaterialTextureSP->SetUniform("material.shininess", 32.0f);
		}
	} else {
		m_pBasicTextureSP->Use();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_bDrawing = true;
}

GLvoid MTextureRenderer::Begin(std::shared_ptr<MskTexture>& texture)
{
	Begin();
	texture->Bind(0);
}

GLvoid MTextureRenderer::Begin(
	std::shared_ptr<MskTexture>& diffuse, 
	std::shared_ptr<MskTexture>& normal, 
	std::shared_ptr<MskTexture>& height
)
{
	// Enable POM
	//m_bPOM = true;

	Begin();
	diffuse->Bind(0);
	normal->Bind(1);
	height->Bind(2);
}

GLvoid MTextureRenderer::End()
{
	if (!m_bDrawing)
		throw std::invalid_argument("MTextureRenderer.Begin() must be called before End().");

	Flush();

	// Disable alpha blending and textures
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Disable shader
	if (m_bLighting) {
		if (m_bPOM) {
			m_pPOMTextureSP->Disable();
		} else {
			m_pMaterialTextureSP->Disable();
		}
	} else {
		m_pBasicTextureSP->Disable();
	}

	m_bDrawing = false;
}

GLvoid MTextureRenderer::Flush()
{
	if (m_vIndirectData.size() == 0 || m_vVertexData.size() == 0)
		return;

	// Vertex Buffer
	glBindVertexArray(m_hVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertexData.size() * sizeof(glm::vec3), &m_vVertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);  // Position

	// Color Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hCBO);
	glBufferData(GL_ARRAY_BUFFER, m_vColor.size() * sizeof(glm::vec4), &m_vColor[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0); // Color
	glVertexAttribDivisor(1, 1); // only once per instance

	// Texture Coords Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hTBO);
	glBufferData(GL_ARRAY_BUFFER, m_vTexCoords.size() * sizeof(glm::vec2), &m_vTexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0); // Text coords

	// Normal Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_hNBO);
	glBufferData(GL_ARRAY_BUFFER, m_vNormalData.size() * sizeof(glm::vec3), &m_vNormalData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);  // Normals

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndexData.size() * sizeof(unsigned short), &m_vIndexData[0], GL_STATIC_DRAW);

	// Indirect Buffer
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_hIndirectBO);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_vIndirectData.size() * sizeof(DrawElementsCommand), &m_vIndirectData[0], GL_STATIC_DRAW);

	// Model Matrix
	glBindBuffer(GL_ARRAY_BUFFER, m_hModelMatrixId);
	glBufferData(GL_ARRAY_BUFFER, m_vModelMatrices.size() * sizeof(glm::mat4), &m_vModelMatrices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	// Tangents
	glBindBuffer(GL_ARRAY_BUFFER, m_hTNBO);
	glBufferData(GL_ARRAY_BUFFER, m_vTangentData.size() * sizeof(glm::vec3), &m_vTangentData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);  // Tangents

	// Bitangents
	glBindBuffer(GL_ARRAY_BUFFER, m_hBNBO);
	glBufferData(GL_ARRAY_BUFFER, m_vBitangentData.size() * sizeof(glm::vec3), &m_vBitangentData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);  // Bitangents

	glMultiDrawElementsIndirect(GL_TRIANGLES,
		GL_UNSIGNED_SHORT,
		nullptr,
		m_vIndirectData.size(),
		0);

	glBindVertexArray(0); // Unbind

	m_vVertexData.clear();
	m_vIndirectData.clear();
	m_vIndexData.clear();
	m_vNormalData.clear();
	m_vTangentData.clear();
	m_vBitangentData.clear();
	m_vDrawId.clear();
	m_vModelMatrices.clear();
	m_vTexCoords.clear();
	m_vColor.clear();
	m_nBaseVertex = 0;
	m_nInstanceBase = 0;
	m_bFlush = false;
}
