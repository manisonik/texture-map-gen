#include "stdafx.h"
#include "msk-gui-view.h"

MView::MView(MskApp* mskApp)
{
	// Initialize Class Variables
	m_hWnd = NULL;
	m_hWndDropTarget = NULL;
	m_isLoaded = FALSE;
	m_rcBounds = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
	m_rcMargins = { 0, 0, 0, 0 };
	m_AdjustedClientRect = { 0, 0, 0, 0 };
	m_nDockFlags = 0;
	m_hBackground = NULL;
	m_hInstance = GetModuleHandle(NULL);
	m_dwStyleEx = WS_EX_APPWINDOW;
	m_dwStyle = WS_OVERLAPPEDWINDOW;
	m_strText = "Default";
	m_strClassName = "MskView";
	m_strMenuName = "";
	m_hIcon = NULL;
	m_hCursor = LoadCursor(NULL, IDC_ARROW);
	this->mskApp = mskApp;

	// Initialize default arg
	eventArgs = new EventArgs();

	// Register the window
	RegisterWindow();

	// Register callbacks
	Destroy += EventHandler<EventArgs>(this, &MView::OnDestroy);
	Render	+= EventHandler<EventArgs>(this, &MView::OnRender);
	Update  += EventHandler<UpdateEventArgs>(this, &MView::OnUpdate);
	Resize  += EventHandler<ResizeEventArgs>(this, &MView::OnResize);
}

MView::~MView()
{
	DeleteObject(m_hBackground);
}

void MView::OnResize(MskObject * sender, ResizeEventArgs * e) {
	for (auto func = m_pControls.begin(); func != m_pControls.end(); ++func) {
		(*func)->Resize(this, e);
	}

	for (auto func = m_pScene.begin(); func != m_pScene.end(); ++func) {
		(*func)->Resize(this, e);
	}

	mskCamera->SetViewport(0, 0, e->m_nWidth, e->m_nHeight);
	mskSSAA->Resize(e->m_nWidth, e->m_nHeight);
}

void MView::OnDestroy(MskObject* sender, EventArgs* e) {
	PostQuitMessage(0);
}

// Clear The Screen And The Depth Buffer
void MView::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MView::OnRender(MskObject* sender, EventArgs * e)
{
	// Enable multisampling
	mskSSAA->Begin();
		// Render screne
		for (auto func = m_pScene.begin(); func != m_pScene.end(); ++func) {
			(*func)->Render(this, eventArgs);
		}
	mskSSAA->End();

	// Render to a quad (probably should include this in the framebuffer class)
	mskTextureRenderer->Begin(mskSSAA->GetTexture());
		mskTextureRenderer->SetCamera(glm::mat4(1.0));
		mskTextureRenderer->Draw(mskQuad);
	mskTextureRenderer->End();
}

void MView::OnUpdate(MskObject* sender, UpdateEventArgs* e)
{
	for (auto func = m_pScene.begin(); func != m_pScene.end(); ++func) {
		(*func)->Update(this, e);
	}
}

int MView::AddControl(std::shared_ptr<MskControl>& control)
{
	m_pControls.push_back(control);
	return 0;
}

GLvoid MView::AddScene(std::shared_ptr<MskScene>& scene)
{
	m_pScene.push_back(scene);
}

GLboolean MView::InitializeGL(GLvoid) {
	// Enable smooth shading
	glShadeModel(GL_SMOOTH);

	// Enable multisampling
	glEnable(GL_MULTISAMPLE);

	// Depth Buffer Setup
	glClearDepth(1.0f);

	// Enables Depth Testing
	glEnable(GL_DEPTH_TEST);

	// Set point size
	glLineWidth(2.0f);

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

HDC MView::GetDeviceContext()
{
	return m_hDC;
}

void MView::RegisterWindow()
{
	// Register window class
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)RouterProc;
	wc.hInstance = m_hInstance;
	wc.hIcon = m_hIcon;
	wc.hCursor = m_hCursor;
	wc.hbrBackground = m_hBackground;
	wc.lpszMenuName = m_strMenuName.c_str();
	wc.lpszClassName = m_strClassName.c_str();

	// Attempt To Register The Window Class
	RegisterClass(&wc);
}

void MView::Show()
{
	CreateWindowEx(m_dwStyleEx, m_strClassName.c_str(), m_strText.c_str(), m_dwStyle, m_rcBounds.X, m_rcBounds.Y,
		m_rcBounds.Width, m_rcBounds.Height, NULL, 0, m_hInstance, this);

	// Show window and update
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

void MView::Hide()
{
	ShowWindow(m_hWnd, SW_HIDE);
}

void MView::SetupOpenGL()
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
		OnDestroy(this, eventArgs);                         // Reset The Display
		fprintf(stderr, "Can't Create A GL Device Context. [ERROR]");
	}

	if (!(m_nPixelFormat = ChoosePixelFormat(m_hDC, &pfd)))   // Did Windows Find A Matching Pixel Format?
	{
		OnDestroy(this, eventArgs);                         // Reset The Display
		fprintf(stderr, "Can't Find A Suitable PixelFormat. [ERROR]");
	}

	if (!SetPixelFormat(m_hDC, m_nPixelFormat, &pfd))         // Are We Able To Set The Pixel Format?
	{
		OnDestroy(this, eventArgs);                         // Reset The Display
		fprintf(stderr, "Can't Set The PixelFormat. [ERROR]");
	}

	if (!(m_hRC = wglCreateContext(m_hDC)))                   // Are We Able To Get A Rendering Context?
	{
		OnDestroy(this, eventArgs);                         // Reset The Display
		fprintf(stderr, "Can't Create A GL Rendering Context. [ERROR]");
	}

	if (!wglMakeCurrent(m_hDC, m_hRC))                        // Try To Activate The Rendering Context
	{
		OnDestroy(this, eventArgs);                         // Reset The Display
		fprintf(stderr, "Can't Activate The GL Rendering Context. [ERROR]");
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	fprintf(stdout, "Status: Using GLEW %s [OK]\n", glewGetString(GLEW_VERSION));

	// Initialize Framebuffer for child controls
	mskApp->CreateFrameRenderer(mskFrameRenderer);
	mskApp->CreateTextureRenderer(mskTextureRenderer);
	mskApp->CreateShapeRenderer(mskShapeRenderer);
	mskApp->CreateLineRenderer(mskLineRenderer);
	mskApp->CreateMultisampleFrameRenderer(mskSSAA);
	mskApp->CreateMesh(mskMeshGrid);
	mskApp->CreateMesh(mskQuad);
	mskApp->CreateMesh(mskCubeMesh);

	// Set grid color
	mskMeshGrid->SetColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

	mskQuad->AddPlane();
	mskCubeMesh->AddCube();
	mskCubeMesh->Scale(1.0f, 1.0f, 1.0f);
	mskCubeMesh->Update();
	mskMeshGrid->AddGrid(50, 50, 50);

	// Get camera
	mskCamera = mskApp->GetCamera();

	mskApp->CreateTexture(mskTexture);
	mskTexture->SetFilter(MskTextureFilter::MipMapLinearLinear, MskTextureFilter::MipMapLinearLinear);
	mskTexture->LoadFromFile("cliff02.png");

	// Create Vertex Shader
	std::shared_ptr<MskShader> vertexShader;
	std::shared_ptr<MskShader> fragmentShader;

	// Create shaders
	MskFactory::CreateShader(MskShaderType::Vertex, vertexShader);
	MskFactory::CreateShader(MskShaderType::Fragment, fragmentShader);
	MskFactory::CreateShaderProgram(m_pShaderProgram);

	vertexShader->LoadFromFile("shader.vs");
	vertexShader->Compile();
	fragmentShader->LoadFromFile("shader.fs");
	fragmentShader->Compile();

	m_pShaderProgram->AttachShader(vertexShader);
	m_pShaderProgram->AttachShader(fragmentShader);
	m_pShaderProgram->Link();

	// Create font and text renderer
	MskFactory::CreateFTFont("C:\\Windows\\fonts\\arial.ttf", mskFont);
	MskFactory::CreateTextRenderer(mskTextRenderer);
}

LRESULT MView::RouterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static MView *view = NULL;

	if (message == WM_NCCREATE) {
		CREATESTRUCT *cs = (CREATESTRUCT*)lParam;
		view = (MView*)cs->lpCreateParams;
		view->m_hWnd = hWnd;

		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)view);
	}
	else view = (MView*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	if (view) return view->OnProcessMessage(message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void MView::SwapBuffers() {
	::SwapBuffers(m_hDC);
}

void MView::MakeCurrent()
{
	wglMakeCurrent(m_hDC, m_hRC);
}

LRESULT MView::OnProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static HDC hdc;
	static EventArgs* eventArgs = new EventArgs();
	static MouseEventArgs* mouseEventArgs = new MouseEventArgs();
	static KeyEventArgs* keyEventArgs = new KeyEventArgs();

	switch (message) {
	case WM_CREATE:
	{
		// Setup the opengl context
		SetupOpenGL();
		InitializeGL();
		Load(this, eventArgs);
	}
	break;
	case WM_MOUSEMOVE:
	{
		mouseEventArgs->x = GET_X_LPARAM(lParam);
		mouseEventArgs->y = GET_Y_LPARAM(lParam);
		mouseEventArgs->buttonDown = m_bMouseDown;
		MouseMove(this, mouseEventArgs);

		for (auto func = m_pScene.begin(); func != m_pScene.end(); ++func) {
			(*func)->MouseMove(this, mouseEventArgs);
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		MouseDown(this, eventArgs);
		m_bMouseDown = GL_TRUE;
	}
	break;
	case WM_LBUTTONUP:
	{
		if (m_bMouseDown) {
			MouseClick(this, eventArgs);
		}
		m_bMouseDown = GL_FALSE;
	}
	break;
	case WM_KEYDOWN:
	{
		keyEventArgs->Key = wParam;
		KeyDown(this, keyEventArgs);
	}
	break;
	case WM_KEYUP:
	{
		keyEventArgs->Key = wParam;
		KeyUp(this, keyEventArgs);
	}
	break;
	case WM_PAINT:
	{
	}
	break;
	case WM_SIZE:
	{
		ResizeEventArgs* args = new ResizeEventArgs();
		args->m_nWidth  = LOWORD(lParam);
		args->m_nHeight = HIWORD(lParam);
		Resize(this, args);
	}
	break;
	case WM_DESTROY:
	{
		Destroy(this, eventArgs);
	}
	break;
	}

	return DefWindowProc(m_hWnd, message, wParam, lParam);
}