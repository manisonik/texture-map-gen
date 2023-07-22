#include "stdafx.h"
#include "msk-gl-mesh-builder.h"

using namespace Manisonik;

MMeshBuilder::MMeshBuilder()
{
    m_nWidth = 100.0;
    m_nHeight = 100.0;
    m_nCount = 100;
    m_nStackCount = 16;
    m_nStackStep = 2 * glm::pi<float>() / m_nStackCount;
    m_nStackAngle = 0; 
    m_nSectorCount = 16;
    m_nSectorStep = 2 * glm::pi<float>() / m_nSectorCount;
    m_nSectorAngle = 0;
    m_nRadius = 0.5f;
}

MMeshBuilder::~MMeshBuilder()
{
}

GLvoid MMeshBuilder::CreateCube(std::shared_ptr<MMesh>& mesh)
{
    // Create mesh
    mesh = std::make_shared<MMesh>();

    // Create manifold
    manifold = std::make_shared<dcel::manifold>();

    // Add vertices
    manifold->insert_vertex(glm::vec3(0.5f, -0.5f, 0.5f));   // 0
    manifold->insert_vertex(glm::vec3(-0.5f, -0.5f, 0.5f));  // 1
    manifold->insert_vertex(glm::vec3(-0.5f, 0.5f, 0.5f));   // 2
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, 0.5f));    // 3
    manifold->insert_vertex(glm::vec3(0.5f, -0.5f, -0.5f));  // 4
    manifold->insert_vertex(glm::vec3(-0.5f, -0.5f, -0.5f)); // 5
    manifold->insert_vertex(glm::vec3(-0.5f, 0.5f, -0.5f));  // 6
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, -0.5f));   // 7

    // Create front face 1
    manifold->insert_face({ 0, 1, 2, 3 });
    // Create right face 2
    manifold->insert_face({ 4, 0, 3, 7 });
    // Create back face 3
    manifold->insert_face({ 4, 7, 6, 5 });
    // Create left face 4
    manifold->insert_face({ 5, 6, 2, 1 });
    // Create top face 5
    manifold->insert_face({ 3, 2, 6, 7 });
    // Create bottom face 6
    manifold->insert_face({ 4, 5, 1, 0 });

    mesh->Set(manifold);
}

GLvoid MMeshBuilder::CreateGrid(std::shared_ptr<MMesh>& mesh)
{
	// Create mesh
	mesh = std::make_shared<MMesh>();

	// Create manifold
	manifold = std::make_shared<dcel::manifold>();

	// Get the dimensions
    float cw = (float)m_nWidth / m_nCount;
    float ch = (float)m_nHeight / m_nCount;
    float hh = (float)m_nHeight / 2;
    float hw = (float)m_nWidth / 2;

    // Insert vertices
    for (int i = 0; i <= m_nCount / 2; i++) {
        manifold->insert_vertex(glm::vec3(i * cw, 0.0f, hh));
        manifold->insert_vertex(glm::vec3(i * cw, 0.0f, -hh));
        manifold->insert_vertex(glm::vec3(-i * cw, 0.0f, hh));
        manifold->insert_vertex(glm::vec3(-i * cw, 0.0f, -hh));
        manifold->insert_vertex(glm::vec3(hw, 0.0f, i * ch));
        manifold->insert_vertex(glm::vec3(-hw, 0.0f, i * ch));
        manifold->insert_vertex(glm::vec3(-hw, 0.0f, -i * ch));
        manifold->insert_vertex(glm::vec3(hw, 0.0f, -i * ch));
    }

    // Create lines
    for (unsigned int i = 0; i < manifold->num_vertices() / 2; i++)
    {
        manifold->insert_face({ i * 2, i * 2 + 1 });
    }

	mesh->Set(manifold);
}

GLvoid MMeshBuilder::CreateCylinder(std::shared_ptr<MMesh>& mesh)
{
    // Create mesh
    mesh = std::make_shared<MMesh>();

    // Create manifold
    manifold = std::make_shared<dcel::manifold>();

    // Insert vertices
    std::vector<uint32_t> idxA, idxB;
    for (uint32_t i = 0; i < m_nSectorCount; i++)
    {
        m_nSectorAngle = i * m_nSectorStep;

        // Bottom
        manifold->insert_vertex(glm::vec3(cos(m_nSectorAngle), -0.5f, sin(m_nSectorAngle)));
        // Top
        manifold->insert_vertex(glm::vec3(cos(m_nSectorAngle),  0.5f, sin(m_nSectorAngle)));

        idxA.push_back(i * 2 + 1);
        idxB.push_back(m_nSectorCount * 2 - i * 2);
    }

    // Insert faces
    for (uint32_t j = 0; j < m_nSectorCount; j++)
    {
        // Draw sides
        if (j == m_nSectorCount-1) 
        {
            manifold->insert_face({ 0, 1, j * 2 + 1, j * 2 });
        }
        else
        {
            manifold->insert_face({ j * 2 + 1, j * 2, j * 2 + 2, j * 2 + 3 });
        }
    }
    // Top face
    manifold->insert_face(idxA);
    //Bottom face
    manifold->insert_face(idxB);

    mesh->Set(manifold);
}

GLvoid MMeshBuilder::CreateSphere(std::shared_ptr<MMesh>& mesh)
{
    // Vertex position
    float x, y, z, xz;

    // Create mesh
    mesh = std::make_shared<MMesh>();

    // Create manifold
    manifold = std::make_shared<dcel::manifold>();

    // Setup stack and sector steps
    m_nStackStep = glm::pi<float>() / m_nStackCount;
    m_nSectorStep = 2 * glm::pi<float>() / m_nSectorCount;

    for (uint32_t i = 0; i <= m_nStackCount; ++i)
    {
        // starting from pi/2 to -pi/2
        m_nStackAngle = glm::pi<float>() / 2 - i * m_nStackStep;

        xz = m_nRadius * cosf(m_nStackAngle); // r * cos(u)
        y = 0.5f * sinf(m_nStackAngle);  // r * sin(u)

        for (int j = 0; j <= m_nSectorCount; ++j)
        {
            m_nSectorAngle = j * m_nSectorStep;  // starting from 0 to 2pi
            x = xz * cosf(m_nSectorAngle); // r * cos(u) * cos(v)
            z = xz * sinf(m_nSectorAngle); // r * cos(u) * sin(v)
            manifold->insert_vertex(glm::vec3(x, y, z));
        }
    }

    uint32_t k1, k2;
    for (uint32_t i = 0; i < m_nStackCount; ++i)
    {
        k1 = i * (m_nSectorCount + 1);     // beginning of current stack
        k2 = k1 + m_nSectorCount + 1;      // beginning of next stack

        for (int j = 0; j < m_nSectorCount; ++j, ++k1, ++k2)
        {
            if (i == 0) {
                manifold->insert_face({ k1 + 1, k2, k2 + 1 });
            } else if (i == m_nStackCount - 1) {
                manifold->insert_face({ k1, k2, k1 + 1 });
            } else {
                manifold->insert_face({ k1, k2, k2 + 1, k1 + 1});
            }
        }
    }

    mesh->Set(manifold);
}

GLvoid Manisonik::MMeshBuilder::CreateIcoSphere(std::shared_ptr<MMesh>& mesh)
{
    // Create mesh
    mesh = std::make_shared<MMesh>();

    // Create manifold
    manifold = std::make_shared<dcel::manifold>();

    // constants
    constexpr float PI = glm::pi<float>();
    const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    std::vector<glm::vec3> vertices(12);    // array of 12 vertices (x,y,z)
    int i1, i2;                             // indices
    float y, xz;                            // coords
    float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 1st row
    float hAngle2 = -PI / 2;                // start from -90 deg at 2nd row

    // the first top vertex at (0, 0, r)
    vertices[0] = glm::vec3(0, -m_nRadius, 0);

    // compute 10 vertices at 1st and 2nd rows
    for (int i = 1; i <= 5; ++i)
    {
        i1 = i;       // index for 1st row
        i2 = i + 5;   // index for 2nd row

        y = m_nRadius * sinf(V_ANGLE);            // elevaton
        xz = m_nRadius * cosf(V_ANGLE);            // length on XY plane

        vertices[i1] = glm::vec3(xz * cosf(hAngle1), -y, xz * sinf(hAngle1));
        vertices[i2] = glm::vec3(xz * cosf(hAngle2), y, xz * sinf(hAngle2));

        // next horizontal angles
        hAngle1 += H_ANGLE;
        hAngle2 += H_ANGLE;
    }

    // the last bottom vertex at (0, 0, -r)
    i1 = 11;
    vertices[i1] = glm::vec3(0, m_nRadius, 0);
     
    for (auto v : vertices) {
        manifold->insert_vertex(v);
    }

    manifold->insert_face({ 1, 5, 0 });
    manifold->insert_face({ 2, 1, 0 });
    manifold->insert_face({ 3, 2, 0 });
    manifold->insert_face({ 4, 3, 0 });
    manifold->insert_face({ 5, 4, 0 });

    manifold->insert_face({ 11, 10, 6 });
    manifold->insert_face({ 11, 6, 7 });
    manifold->insert_face({ 11, 7, 8 });
    manifold->insert_face({ 11, 8, 9 });
    manifold->insert_face({ 11, 9, 10 });


    manifold->insert_face({ 5, 1, 10 });
    manifold->insert_face({ 6, 1, 2 });
    manifold->insert_face({ 7, 2, 3 });
    manifold->insert_face({ 8, 3, 4 });
    manifold->insert_face({ 9, 4, 5 });

    manifold->insert_face({ 1, 6, 10 });
    manifold->insert_face({ 2, 7, 6 });
    manifold->insert_face({ 3, 8, 7 });
    manifold->insert_face({ 4, 9, 8 });
    manifold->insert_face({ 5, 10, 9 });


    /*for (uint32_t i = 0; i < 4; i++) {
        manifold->insert_face({ i * 3, i * 3 + 1 , i * 3 + 2 });
    }*/

    // Set manifold
    mesh->Set(manifold);
}

GLvoid MMeshBuilder::CreateTest(std::shared_ptr<MMesh>& mesh)
{
    // Create mesh
    mesh = std::make_shared<MMesh>();

    // Create manifold
    manifold = std::make_shared<dcel::manifold>();


    manifold->insert_vertex(glm::vec3(0.5f, -0.5f, 0.5f));   // 0
    manifold->insert_vertex(glm::vec3(-0.5f, -0.5f, 0.5f));  // 1
    manifold->insert_vertex(glm::vec3(-0.5f, 0.5f, 0.5f));   // 2
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, 0.5f));    // 3

    manifold->insert_vertex(glm::vec3(0.5f, -0.5f, -0.5f));  // 4
    manifold->insert_vertex(glm::vec3(-0.5f, -0.5f, -0.5f)); // 5
    manifold->insert_vertex(glm::vec3(-0.5f, 0.5f, -0.5f));  // 6
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, -0.5f));   // 7



    /*manifold->insert_vertex(glm::vec3(-0.5f, -0.5f, -0.5f));    // 0
    manifold->insert_vertex(glm::vec3(-0.5f,  0.5f, -0.5f));    // 1
    manifold->insert_vertex(glm::vec3( 0.5f,  0.5f, -0.5f));    // 2
    manifold->insert_vertex(glm::vec3(0.5f, 0.125f, -0.5f));    // 3
    manifold->insert_vertex(glm::vec3(-0.25f, 0.125f, -0.5f));  // 4
    manifold->insert_vertex(glm::vec3(-0.25f, -0.125f, -0.5f)); // 5
    manifold->insert_vertex(glm::vec3(0.5f, -0.125f, -0.5f));  // 6
    manifold->insert_vertex(glm::vec3(0.5f, -0.5f, -0.5f));     // 7
  
    manifold->insert_vertex(glm::vec3(0.5f, -0.5f, 0.5f));    // 8
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, 0.5f));    // 9
    manifold->insert_vertex(glm::vec3(-0.5f, 0.5f, 0.5f));    // 10
    manifold->insert_vertex(glm::vec3(-0.5f, 0.125f, 0.5f));    // 11
    manifold->insert_vertex(glm::vec3(0.25f, 0.125f, 0.5f));  // 12
    manifold->insert_vertex(glm::vec3(0.25f, -0.125f, 0.5f)); // 13
    manifold->insert_vertex(glm::vec3(-0.5f, -0.125f, 0.5f));  // 14
    manifold->insert_vertex(glm::vec3(-0.5f, -0.5f, 0.5f));     // 15*/

    manifold->insert_face({ 0, 1, 2, 3 });
    manifold->insert_face({ 4, 7, 6, 5 });

    //manifold->insert_face({ 0, 1, 2, 3, 4, 5, 6, 7 });
    //manifold->insert_face({ 8, 9, 10, 11, 12, 13, 14, 15 });

    /*manifold->insert_vertex(glm::vec3(-0.5f, 0.5f, -0.5f));    // 0
    manifold->insert_vertex(glm::vec3(-0.5f, 0.5f, 0.5f));    // 1
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, 0.5f));    // 2
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, 0.125f));    // 3
    manifold->insert_vertex(glm::vec3(-0.25f, 0.5f, 0.125f));  // 4
    manifold->insert_vertex(glm::vec3(-0.25f, 0.5f, -0.125f)); // 5
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, -0.125f));  // 6
    manifold->insert_vertex(glm::vec3(0.5f, 0.5f, -0.5f));     // 7*/

    /*manifold->insert_vertex(glm::rotateZ(glm::vec3(-0.5f, 0.5f, -0.5f), -90.0f));    // 0
    manifold->insert_vertex(glm::rotateZ(glm::vec3(-0.5f, 0.5f, 0.5f), -90.0f));    // 1
    manifold->insert_vertex(glm::rotateZ(glm::vec3(0.5f, 0.5f, 0.5f), -90.0f));    // 2
    manifold->insert_vertex(glm::rotateZ(glm::vec3(0.5f, 0.5f, 0.125f), -90.0f));    // 3
    manifold->insert_vertex(glm::rotateZ(glm::vec3(-0.25f, 0.5f, 0.125f), -90.0f));  // 4
    manifold->insert_vertex(glm::rotateZ(glm::vec3(-0.25f, 0.5f, -0.125f), -90.0f)); // 5
    manifold->insert_vertex(glm::rotateZ(glm::vec3(0.5f, 0.5f, -0.125f), -90.0f));  // 6
    manifold->insert_vertex(glm::rotateZ(glm::vec3(0.5f, 0.5f, -0.5f), -90.0f));     // 7*/

   

    mesh->Set(manifold);
}
