#include "stdafx.h"
#include "msk-gl-mesh-builder.h"
#include "msk-gl-mesh.h"

MMskMeshBuilder::MMskMeshBuilder(MskApp* mskApp)
{
}

MMskMeshBuilder::~MMskMeshBuilder()
{
}

GLvoid MMskMeshBuilder::CreateCyclinder(std::shared_ptr<MskMesh>& mesh)
{
	std::shared_ptr<MMesh> meshData = std::make_shared<MMesh>();

	// Generate unit circle
	GLuint sectorCount = 32;
	GLuint sliceCount = 1;
	float sectorStep = 2 * M_PI / sectorCount;
	float sectorAngleA = 0;  // radian
	float sectorAngleB = 0;  // radian

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<MskFace> faces;
	std::vector<glm::vec2> texCoords;

	// Create sides
	for (int i = 0; i <= sectorCount; i++)
	{
		sectorAngleA = i * sectorStep;
		sectorAngleB = (i + 1) * sectorStep;

		vertices.push_back(glm::vec3(cos(sectorAngleA), -0.5f, sin(sectorAngleA)));
		vertices.push_back(glm::vec3(cos(sectorAngleA), 0.5f, sin(sectorAngleA)));
		vertices.push_back(glm::vec3(cos(sectorAngleB), -0.5f, sin(sectorAngleB)));

		vertices.push_back(glm::vec3(cos(sectorAngleB), -0.5f, sin(sectorAngleB)));
		vertices.push_back(glm::vec3(cos(sectorAngleA), 0.5f, sin(sectorAngleA)));
		vertices.push_back(glm::vec3(cos(sectorAngleB), -0.5f, sin(sectorAngleB)));

		glm::vec3 normal = glm::triangleNormal(
			glm::vec3(vertices[i]),
			glm::vec3(vertices[i + 1]),
			glm::vec3(vertices[i + 2])
		);
		normals.insert(normals.end(), 3, normal);

		normal = glm::triangleNormal(
			glm::vec3(vertices[i]),
			glm::vec3(vertices[i + 1]),
			glm::vec3(vertices[i + 2])
		);
		normals.insert(normals.end(), 3, normal);
	}

	// Create center vertices
	vertices.push_back(glm::vec3(0, -0.5f, 0));
	vertices.push_back(glm::vec3(0, -0.5f, 0));

	// Create top and bottom
	for (int i = 0; i <= sectorCount; i++)
	{
		sectorAngleA = i * sectorStep;
		vertices.push_back(glm::vec3(cos(sectorAngleA), -0.5f, sin(sectorAngleA)));
		vertices.push_back(glm::vec3(cos(sectorAngleA), 0.5f, sin(sectorAngleA)));
	}


	// Create bottom and top faces
	for (GLuint i = 1; i <= sectorCount; i++)
	{
		// Bottom
		faces.push_back({ {i, i + 1, 0}, {0, 1, 0, 2, 2, 3} });

		// Top
		faces.push_back({ {sectorCount + i, sectorCount + i + 1, sectorCount}, {0, 1, 0, 2, 2, 3} });
	}

	// Draw sides
	for (GLuint i = 1; i <= sectorCount; i++)
	{
		faces.push_back({ {i + 1, i, sectorCount + i + 3, sectorCount + i + 3,  i, sectorCount + i + 2}, {0, 1, 0, 2, 2, 3, 1, 3} });
		normals.insert(normals.end(), 3, glm::vec3(0, 0, -1));
	}

	meshData->SetVertices(vertices);
	meshData->SetFaces(faces);
	meshData->SetTextureCoords(texCoords);
	meshData->SetNormals(normals);
	meshData->CalculateTangentBasis();

	mesh = meshData;
}

GLvoid MMskMeshBuilder::CreateCube(std::shared_ptr<MskMesh>& mesh)
{
	std::shared_ptr<MMesh> meshData = std::make_shared<MMesh>();
	std::vector<glm::vec3> vertices = {
		// Back
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),

		// Front
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),

		// Left
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),

		// Right
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),

		// Bottom
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),

		// Top
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
	};
	std::vector<glm::vec3> normals =
	{
		// Back
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		// Front
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		// Left
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		// Right
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		// Bottom
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		// Top
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
	};
	std::vector<glm::vec2> texCoords = {
		// Back
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Front
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Left
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Right
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Bottom
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3

		// Top
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
	};
	std::vector<GLuint> indices = {
		0, 1, 2, 2, 1, 3, // Back
		4, 5, 6, 6, 5, 7, // Front
		8, 9, 10, 10, 9, 11, // Left
		12, 13, 14, 14, 13, 15, // Right
		16, 17, 18, 18, 17, 19, // Bottom
		20, 21, 22, 22, 21, 23  // Top
	};
	
	std::vector<MskFace> faces;
	faces.push_back({ { 0, 1, 2, 2, 1, 3},       {0, 1, 0, 2, 2, 3, 1, 3 } });
	faces.push_back({ {4, 5, 6, 6, 5, 7 },       {0, 1, 0, 2, 2, 3, 1, 3 } });
	faces.push_back({ {8, 9, 10, 10, 9, 11 },    {0, 1, 0, 2, 2, 3, 1, 3 } });
	faces.push_back({ {12, 13, 14, 14, 13, 15 }, {0, 1, 0, 2, 2, 3, 1, 3 } });
	faces.push_back({ {16, 17, 18, 18, 17, 19 }, {0, 1, 0, 2, 2, 3, 1, 3 } });
	faces.push_back({ {20, 21, 22, 22, 21, 23 }, {0, 1, 0, 2, 2, 3, 1, 3 } });

	meshData->SetVertices(vertices);
	meshData->SetIndices(indices);
	meshData->SetFaces(faces);
	meshData->SetTextureCoords(texCoords);
	meshData->SetNormals(normals);
	meshData->CalculateTangentBasis();

	// Output mesh data
	mesh = meshData;
}

GLvoid MMskMeshBuilder::CreateCone(std::shared_ptr<MskMesh>& mesh)
{
	return GLvoid();
}

GLvoid MMskMeshBuilder::CreatePlane(std::shared_ptr<MskMesh>& mesh)
{
	std::shared_ptr<MMesh> meshData = std::make_shared<MMesh>();

	std::vector<glm::vec3> vertices = {
		glm::vec3(-1.0f, -1.0f, 0.0f), // 0
		glm::vec3(1.0f, -1.0f, 0.0f),  // 1
		glm::vec3(-1.0f, 1.0f, 0.0f),  // 2
		glm::vec3(1.0f, 1.0f, 0.0f),
	};

	std::vector<glm::vec2> texCoords = {
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(1.0f, 0.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(1.0f, 1.0f), // 3
	};

	std::vector<glm::vec3> normals = {

	};

	meshData->SetVertices(vertices);
	meshData->SetTextureCoords(texCoords);
	meshData->SetNormals(normals);
	meshData->CalculateTangentBasis();

	// Output mesh data
	mesh = meshData;
}

GLvoid MMskMeshBuilder::CreateIcoSpere(std::shared_ptr<MskMesh>& mesh)
{
	return GLvoid();
}

GLvoid MMskMeshBuilder::CreateTorus(std::shared_ptr<MskMesh>& mesh)
{
	return GLvoid();
}

GLvoid MMskMeshBuilder::CreateCircle(std::shared_ptr<MskMesh>& mesh)
{
	return GLvoid();
}

GLvoid MMskMeshBuilder::CreateUVSphere(std::shared_ptr<MskMesh>& mesh)
{
	return GLvoid();
}

GLvoid MMskMeshBuilder::CreateGrid(std::shared_ptr<MskMesh>& mesh)
{
	return GLvoid();
}