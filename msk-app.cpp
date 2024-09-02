#include "stdafx.h"
#include "msk-app.h"

using namespace TexGen;
using namespace std::placeholders;

Tg::Tg(std::unique_ptr<ITgWindow>&& window) : 
    m_window(std::move(window))
{ 
    // Initialize opengl extensions
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        const GLubyte* s = glewGetErrorString(err);
        std::runtime_error("Failed to init GLEW");
    }

    // Enable smooth shading
    glShadeModel(GL_SMOOTH);

    // Enable multisampling
    glEnable(GL_MULTISAMPLE);

    // Depth Buffer Setup
    glClearDepth(1.0f);

    // Enables Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Set point size
    glLineWidth(1.5f);

    // Use square points
    glEnable(GL_POINT_SPRITE);

    // Enable point size
    glEnable(GL_PROGRAM_POINT_SIZE);

    glFrontFace(GL_CW);

    // Render front and back
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Really Nice Perspective Calculations
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Enable Blending
    glEnable(GL_BLEND);

    // Type Of Blending To Use
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Culling
    glEnable(GL_CULL_FACE);
}

Tg::~Tg()
{ 
}

void Tg::addScene(const std::shared_ptr<ITgScene>& scene)
{
    m_window->addScene(scene);
}

void Tg::run()
{
    m_window->run();
}

void Tg::update(float dt)
{
    m_window->update(dt);
}

void Tg::render()
{ 
    m_window->render();
}