#include "stdafx.h"
#include "msk-glfw-window.h"

using namespace TexGen;

TgGlfwindow::TgGlfwindow(int width, int height) :
    m_window(nullptr)
{
    /* Initialize the library */
    if (!glfwInit())
        throw std::runtime_error("GLFW failed to initialize.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("GLFW failed to create window");
    }

    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

    //glfwSetKeyCallback(m_window.get(), TgGlfwindow::keyCallback);
    //glfwSetCursorPosCallback(m_window.get(), TgGlfwindow::mouseCallback);
    //glfwSetScrollCallback(m_window.get(), TgGlfwindow::scrollCallback);

    // Make the window's context current
    glfwMakeContextCurrent(m_window);

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
    //glEnable(GL_POINT_SPRITE);

    // Enable point size
    glEnable(GL_PROGRAM_POINT_SIZE);

    glFrontFace(GL_CW);

    // Render front and back
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Really Nice Perspective Calculations
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Enable Blending
    glEnable(GL_BLEND);

    // Type Of Blending To Use
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Culling
    glEnable(GL_CULL_FACE);
}

TgGlfwindow::~TgGlfwindow()
{
    glfwTerminate();
}

void TgGlfwindow::show()
{
    glfwShowWindow(m_window);
}

void TgGlfwindow::hide()
{
    glfwHideWindow(m_window);
}

void TgGlfwindow::run()
{
    double frameRate = 60;
    double frameInterval = 1 / frameRate;
    double startTime = 0;
    double lastTime = glfwGetTime();
    double timer = lastTime;
    int frames = 0;
    int updates = 0;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(m_window))
    {
        auto startTime = glfwGetTime();
        auto deltaTime = startTime - lastTime;
        lastTime = startTime;
 
        /*while (deltaTime >= 1.0){
           
            updates++;
            deltaTime--;
        }*/

        update(deltaTime);

        // Render here
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        render();
        frames++;

        // Output timer
        if (glfwGetTime() - timer > 1.0) {
            timer++;
            std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
            updates = 0; 
            frames = 0;
        }

        // Swap front and back buffers
        glfwSwapBuffers(m_window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
}

void TgGlfwindow::addScene(const std::shared_ptr<ITgScene>& scene)
{
    m_scene.push_back(scene);
}

void TgGlfwindow::update(float dt)
{
    for (auto& scene : m_scene)
    {
        scene->update(dt);
    }
}

void TgGlfwindow::render()
{
    for (auto& scene : m_scene)
    {
        scene->render();
    }
}

void TgGlfwindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
     TgGlfwindow* handler = reinterpret_cast<TgGlfwindow*>(glfwGetWindowUserPointer(window));
     //if (handler != nullptr)
        //handler->
}

void TgGlfwindow::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
}

void TgGlfwindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}
