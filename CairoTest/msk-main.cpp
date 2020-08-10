// CairoTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "msk-main.h"
#include "msk-glm-camera.h"
#include "msk-gl-shaderprogram.h"
#include "msk-cairo.h"
#include "msk-gl-texture.h"
#include "msk-gl-shaperenderer.h"
#include "msk-core.h"
#include "msk-gui-image.h"

#define MAX_LOADSTRING 100
#define CAIRO_TEXTURE_SIZE 1024

// Constants
#define PI 3.14159
#define DELTA 5.0f;

// Camera variables
int mouseLastx;
int mouseLasty;
float cam_x;
float cam_y;
float cam_z;
float cam_rot_x;
float cam_rot_y;
float cam_rot_z;
float strafe;
float walk;
bool mouseDown = false;
bool lookUp;
bool lookDown;
bool lookLeft;
bool lookRight;

// Global Variables:
HINSTANCE g_hInst;                                // current instance
CHAR szTitle[MAX_LOADSTRING];                  // The title bar text
CHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HGLRC g_hRC;
HDC g_hDC;
HWND g_hWnd;
unsigned char*	g_SurfaceData;
UINT g_nWidth = 800;
UINT g_nHeight = 600;
UINT g_TextureId;
GLuint g_FrameBuffer;

std::shared_ptr<MCamera> g_pCamera = std::shared_ptr<MCamera>(new MCamera());
MShaderProgram* shaderProgram = NULL;
MCairo* g_Cairo = NULL;
MTexture* g_Texture = NULL;
MShapeRenderer* g_pShapeRenderer = NULL;
MImage* g_Image = NULL;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void CairoDrawSurface();
void ResizeGL(int width, int height);
void RenderScene();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::shared_ptr<MskApp> mskApp;
	std::shared_ptr<MskImage> mskImage;

	MskFactory::CreateApplication(mskApp);

	return mskApp->Run();
}

void ResizeGL(int width, int height) {
	// Resize the viewport
	g_pCamera->SetViewport(0, 0, width / 2, height / 2);
}

void CairoDrawSurface(void) {
	g_Cairo->Clear(1.0f, 1.0f, 1.0f, 1.0f);

	g_Cairo->Rectangle(0, 0, g_Cairo->GetWidth(), g_Cairo->GetHeight());
	//g_Cairo->FillRectangle(5, 5, 100, 100);
	g_Cairo->Circle(0, 0, 50, 100);
	g_Cairo->Triangle(256, 256, 500, 500);

	// Fill texture data
	//g_Texture->SetData(g_Cairo->GetSurfaceData());
}

cairo_t* create_cairo_context(
	HDC hDC,
	HGLRC hRC,
	int width,
	int height,
	unsigned int textureId,
	cairo_surface_t** surf
) {
	cairo_device_t* device = cairo_wgl_device_create(hRC);

	*surf = cairo_gl_surface_create_for_dc(device, hDC, width, height);

	if (cairo_surface_status(*surf) != CAIRO_STATUS_SUCCESS)
	{
		printf("create_cairo_context() - Couldn't create surface\n");
		return NULL;
	}

	cairo_t* cr = cairo_create(*surf);
	if (cairo_status(cr) != CAIRO_STATUS_SUCCESS)
	{
		printf("create_cairo_context() - Couldn't create context\n");
		return NULL;
	}

	cairo_device_destroy(device);

	//wglMakeCurrent(hDC, hRC);

	// FIX
	//cairo_gl_surface_swapbuffers(g_Surface);

	return cr;
}

GLvoid KillGLWindow(GLvoid) {
	// Do We Have A Rendering Context?
	if (g_hRC) {
		// Are We Able To Release The DC And RC Contexts?
		if (!wglMakeCurrent(NULL, NULL)) {
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		// Are We Able To Delete The RC?
		if (!wglDeleteContext(g_hRC)) {
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		// Set RC To NULL
		g_hRC = NULL;
	}

	// Are We Able To Release The DC
	if (g_hRC && !ReleaseDC(g_hWnd, g_hDC)) {
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		// Set DC To NULL
		g_hDC = NULL;
	}

	// Are We Able To Destroy The Window?
	if (g_hWnd && !DestroyWindow(g_hWnd)) {
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		// Set hWnd To NULL
		g_hWnd = NULL;
	}
}

void RenderScene() {
	// Draw cairo surface
	CairoDrawSurface();

	// Update the camera
	g_pCamera->Update(1);

	// Clear backgound
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	g_pShapeRenderer->Begin();
	g_pShapeRenderer->SetProjectionMatrix(g_pCamera->GetModelViewProjectionMatrix());
	//g_pShapeRenderer->DrawGrid(101, 101, 101);
	g_pShapeRenderer->End();

	SwapBuffers(g_hDC);
}

void InitCairo(int width, int height) {
	g_Cairo = new MCairo();
}

void InitGL() {
	// Enable smooth shading
	glShadeModel(GL_SMOOTH);

	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Depth Buffer Setup
	glClearDepth(1.0f);

	// Enables Depth Testing
	glEnable(GL_DEPTH_TEST);

	// Really Nice Perspective Calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	// Set camera position
	g_pCamera->SetPosition(glm::vec3(0.0f, 0.0f, 7.0f));

	// Create texture
	//g_Texture = new MTexture(
		//g_Cairo->GetWidth(),
		//g_Cairo->GetHeight(),
		//g_Cairo->GetSurfaceData()
	//);
}

void CreateGL() {
	GLuint PixelFormat;		// Holds The Results After Searching For A Match

	// Register Opengl class
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;			// Redraw On Move, And Own DC For Window
	wc.lpfnWndProc = (WNDPROC)WndProc;						// WndProc Handles Messages
	wc.cbClsExtra = 0;										// No Extra Window Data
	wc.cbWndExtra = 0;										// No Extra Window Data
	wc.hInstance = g_hInst;									// Set The Instance
	wc.hIcon = 0;											// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				// Load The Arrow Pointer
	wc.hbrBackground = NULL;								// No Background Required For GL
	wc.lpszMenuName = MAKEINTRESOURCE(IDC_CAIROTEST); // We Don't Want A Menu
	wc.lpszClassName = szWindowClass;						   // Set The Class Name
	wc.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_CAIROTEST));

	// Register class;														
	RegisterClass(&wc);

	CREATESTRUCT cs;
	ZeroMemory(&cs, sizeof(CREATESTRUCT));
	cs.x = 0;
	cs.y = 0;
	cs.cx = 800;
	cs.cy = 600;
	cs.lpszName = "CairoGL";
	cs.hInstance = g_hInst;
	cs.lpszClass = szWindowClass;
	cs.style = WS_OVERLAPPEDWINDOW;
	cs.lpCreateParams = NULL;
	cs.hMenu = NULL;

	g_hWnd = CreateWindowEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style,
		cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

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

	if (!(g_hDC = GetDC(g_hWnd)))                         // Did We Get A Device Context?
	{
		KillGLWindow();                         // Reset The Display
		fprintf(stderr, "Can't Create A GL Device Context. [ERROR]");
	}

	if (!(PixelFormat = ChoosePixelFormat(g_hDC, &pfd)))             // Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();                         // Reset The Display
		fprintf(stderr, "Can't Find A Suitable PixelFormat. [ERROR]");
	}

	if (!SetPixelFormat(g_hDC, PixelFormat, &pfd))               // Are We Able To Set The Pixel Format?
	{
		KillGLWindow();                         // Reset The Display
		fprintf(stderr, "Can't Set The PixelFormat. [ERROR]");
	}

	if (!(g_hRC = wglCreateContext(g_hDC)))                   // Are We Able To Get A Rendering Context?
	{
		KillGLWindow();                         // Reset The Display
		fprintf(stderr, "Can't Create A GL Rendering Context. [ERROR]");
	}

	if (!wglMakeCurrent(g_hDC, g_hRC))                        // Try To Activate The Rendering Context
	{
		KillGLWindow();                         // Reset The Display
		fprintf(stderr, "Can't Activate The GL Rendering Context. [ERROR]");
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	// Load shaders
	/*MShader* vertexShader = new MShader(GL_VERTEX_SHADER);
	vertexShader->LoadFromFile("shader.vs");
	vertexShader->Compile();

	MShader* fragmentShader = new MShader(GL_FRAGMENT_SHADER);
	fragmentShader->LoadFromFile("shader.fs");
	fragmentShader->Compile();

	// Compile shaders
	shaderProgram = new MShaderProgram();
	shaderProgram->AttachShader(vertexShader);
	shaderProgram->AttachShader(fragmentShader);
	shaderProgram->Link();*/
	
	shaderProgram->AddUniform("MVP");

	// Create Shape Object
	g_pShapeRenderer = new MShapeRenderer();

	// Create Cairo
	InitCairo(cs.cx, cs.cy);

	// Initialize OpenGL
	InitGL();
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAIROTEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_CAIROTEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// Store instance handle in our global variable
	g_hInst = hInstance;

	// Create GL
	CreateGL();

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_SIZE:
		g_nWidth = LOWORD(lParam);
		g_nHeight = HIWORD(lParam);
		ResizeGL(g_nWidth, g_nHeight);
		break;
	case WM_LBUTTONDOWN:
		mouseDown = true;
		break;
	case WM_LBUTTONUP:
		mouseDown = false;
		break;
	case WM_MOUSEMOVE:
		g_pCamera->Look(LOWORD(lParam), HIWORD(lParam), true);
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'W':
			g_pCamera->MoveForward(false);
			break;
		case 'S':
			g_pCamera->MoveBackward(false);
			break;
		case 'A':
			g_pCamera->MoveLeft(false);
			break;
		case 'D':
			g_pCamera->MoveRight(false);
			break;
		case 'w':
			g_pCamera->MoveForward(false);
			break;
		case 's':
			g_pCamera->MoveBackward(false);
			break;
		case 'a':
			g_pCamera->MoveLeft(false);
			break;
		case 'd':
			g_pCamera->MoveRight(false);
			break;
		default:
			break;
		}
		break;
	case WM_CHAR:
		switch (wParam) {
		case 'W':
			g_pCamera->MoveForward(true);
			break;
		case 'S':
			g_pCamera->MoveBackward(true);
			break;
		case 'A':
			g_pCamera->MoveLeft(true);
			break;
		case 'D':
			g_pCamera->MoveRight(true);
			break;
		case 'w':
			g_pCamera->MoveForward(true);
			break;
		case 's':
			g_pCamera->MoveBackward(true);
			break;
		case 'a':
			g_pCamera->MoveLeft(true);
			break;
		case 'd':
			g_pCamera->MoveRight(true);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
