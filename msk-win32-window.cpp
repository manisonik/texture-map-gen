#include "stdafx.h"
#include "msk-win32-window.h"

using namespace TexGen;

TgWin32Window::TgWin32Window()
{
}

TexGen::TgWin32Window::~TgWin32Window()
{
    ReleaseDC(m_hWnd, m_hDC);
    wglDeleteContext(m_hRC);
}

void TexGen::TgWin32Window::show()
{
}

void TexGen::TgWin32Window::hide()
{
}

void TexGen::TgWin32Window::addScene(const std::shared_ptr<ITgScene>& scene)
{
}

void TgWin32Window::registerWindow()
{
    // Register window class
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)routerProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = "";
    wc.lpszClassName = "MskView";

    // Attempt To Register The Window Class
    RegisterClass(&wc);
}

LRESULT TgWin32Window::routerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TgWin32Window* view = NULL;

	if (message == WM_NCCREATE) {
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		view = (TgWin32Window*)cs->lpCreateParams;
		view->m_hWnd = hWnd;

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)view);
	}
	else view = (TgWin32Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (view)
        return view->onProcessMessage(message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool TgWin32Window::setupOpenGL()
{
    static  PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
        1,												// Version Number
        PFD_DRAW_TO_WINDOW |							// Format Must Support Window
        PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
        PFD_DOUBLEBUFFER,								// Must Support Double Buffering
        PFD_TYPE_RGBA,									// Request An RGBA Format
        16,												// Select Our Color Depth
        0, 0, 0, 0, 0, 0,								// Color Bits Ignored
        0,												// No Alpha Buffer
        0,												// Shift Bit Ignored
        0,												// No Accumulation Buffer
        0, 0, 0, 0,										// Accumulation Bits Ignored
        16,												// 16Bit Z-Buffer (Depth Buffer)
        0,												// No Stencil Buffer
        0,												// No Auxiliary Buffer
        PFD_MAIN_PLANE,									// Main Drawing Layer
        0,												// Reserved
        0, 0, 0											// Layer Masks Ignored
    };

    if (!(m_hDC = GetDC(m_hWnd)))                         // Did We Get A Device Context?
    {
        fprintf(stderr, "Can't Create A GL Device Context. [ERROR]");
        return false;
    }

    if (!(m_nPixelFormat = ChoosePixelFormat(m_hDC, &pfd)))   // Did Windows Find A Matching Pixel Format?
    {
        fprintf(stderr, "Can't Find A Suitable PixelFormat. [ERROR]");
        return false;
    }

    if (!SetPixelFormat(m_hDC, m_nPixelFormat, &pfd))         // Are We Able To Set The Pixel Format?
    {
        fprintf(stderr, "Can't Set The PixelFormat. [ERROR]");
        return false;
    }

    if (!(m_hRC = wglCreateContext(m_hDC)))                   // Are We Able To Get A Rendering Context?
    {
        fprintf(stderr, "Can't Create A GL Rendering Context. [ERROR]");
        return false;
    }

    if (!wglMakeCurrent(m_hDC, m_hRC))                        // Try To Activate The Rendering Context
    {
        fprintf(stderr, "Can't Activate The GL Rendering Context. [ERROR]");
        return false;
    }

    return true;
}

bool TgWin32Window::initializeGL() {
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

    return true;
}

LRESULT TgWin32Window::onProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    //static MMouseEventArgs* mouseEventArgs = new MMouseEventArgs();
    //static MResizeEventArgs* resizeEventArgs = new MResizeEventArgs();
    //static MKeyEventArgs* keyEventArgs = new MKeyEventArgs();

    switch (message) {
    case WM_CREATE:
    {
        setupOpenGL();
        initializeGL();

        /*for (auto &scene : m_pScenes) {
            scene->initialize();
        }*/
    }
    break;
    case WM_SIZE:
    {
        uint32_t width = LOWORD(lParam);
        uint32_t height = HIWORD(lParam);
        //m_pMSAA.resize(width, height);
        /*for (auto &scene : m_pScenes) {
            //scene->ResizeEvent(this, resizeEventArgs);
        }*/
       
    }
    break;
    case WM_LBUTTONDOWN:
    {
        m_bMouseDown = true;
        uint32_t x = GET_X_LPARAM(lParam);
        uint32_t y = GET_Y_LPARAM(lParam);
        //bool button = static_cast<MouseButton>(wParam);
        /*for (auto &scene : m_pScenes) {
            //scene->MouseMoveEvent(this, mouseEventArgs);
        }*/
    }
    break;
    case WM_LBUTTONUP:
    {   
        uint32_t x = GET_X_LPARAM(lParam);
        uint32_t y = GET_Y_LPARAM(lParam);
        //mouseEventArgs->button = static_cast<MouseButton>(wParam);
        /*for (auto &scene : m_pScenes) {
            //scene->MouseUpEvent(this, mouseEventArgs);
            if (m_bMouseDown) {
                //scene->MouseClickEvent(this, mouseEventArgs);
            }
        }*/
    }
    break;
    case WM_MOUSEMOVE:
    { 
        uint32_t x = GET_X_LPARAM(lParam);
        uint32_t y = GET_Y_LPARAM(lParam);
        //mouseEventArgs->button = static_cast<MouseButton>(wParam);
        /*for (auto &scene : m_pScenes) {
            //scene->MouseMoveEvent(this, mouseEventArgs);
        }*/
    }
    break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;
    case WM_KEYDOWN:
    {
        //Keys keyCode = (Keys)wParam;
        /*keyEventArgs->KeyCode = keyCode;
        keyEventArgs->Shift = keyCode == Keys::Shift;
        keyEventArgs->Alt = false;
        keyEventArgs->Control = keyCode == Keys::Control;*/
        /*for (auto &scene : m_pScenes) {
            //scene->KeyDownEvent(this, keyEventArgs);
        }*/
    }
    break;
    case WM_KEYUP:
    {
        //Keys keyCode = (Keys)wParam;
        /*keyEventArgs->KeyCode = keyCode;
        keyEventArgs->Shift = keyCode == Keys::Shift;
        keyEventArgs->Alt = false;
        keyEventArgs->Control = keyCode == Keys::Control;*/
        /*for (auto &scene : m_pScenes) {
            //scene->keyUpEvent(this, keyEventArgs);
        }*/
    }
    break;
    }

    return DefWindowProc(m_hWnd, message, wParam, lParam);
}

void TgWin32Window::run()
{
    MSG msg;
    BOOL done = FALSE;

    // Start timer
    auto startTime = std::chrono::system_clock::now();
    float deltaTime = 0;

    float frameRate = 30;
    float frameInterval = 1 / frameRate;
    float time = 0;

    while (!done)
    {
        auto endTime = std::chrono::system_clock::now();
        std::chrono::duration<float, std::milli> dt = endTime - startTime;
        time += dt.count();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                done = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            update(dt.count());

            if (time > frameInterval) {
                render();
                time = 0;
            }
        }

        // Reset time
        startTime = std::chrono::system_clock::now();
    }
}

void TgWin32Window::update(float dt)
{

}

void TgWin32Window::render()
{ 
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //m_pMSAA->begin();
    /*for (auto &scene : m_pScenes) {
        scene->render();
    }*/
    //m_pMSAA->end();

    //SwapBuffers(m_hDC);
}