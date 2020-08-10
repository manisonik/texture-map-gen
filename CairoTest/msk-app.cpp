#include "stdafx.h"
#include "msk-core.h"
#include "msk-app.h"
#include "msk-gui-control.h"
#include "msk-gui-view.h"
#include "msk-gui-oglwindow.h"
#include "msk-gui-image.h"
#include "msk-gl-shader.h"
#include "msk-gl-shaderprogram.h"
#include "msk-gl-font.h"
#include "msk-gl-textrenderer.h"
#include "msk-gl-shaperenderer.h"
#include "msk-gl-linerenderer.h"
#include "msk-glm-camera.h"
#include "msk-gl-texture.h"
#include "msk-gl-texturerenderer.h"
#include "msk-gl-mesh.h"
#include "msk-gl-framerenderer.h"
#include "msk-gl-multisampleframerenderer.h"
#include "msk-cairo.h"
#include "msk-gl-scene-filemenu.h"

MApplication::MApplication() : 
	m_nZoom(0.5f)
{
	// Create the view
	CreateView(mskView);

	// Create the camera
	CreateCamera(mskCamera);

	mskCamera->SetMode(1);
	mskCamera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));

	// Callbacks
	mskView->Load += EventHandler<EventArgs>(this, &MApplication::OnLoad);
}

MApplication::~MApplication(){}

GLvoid MApplication::CreateControl(std::shared_ptr<MskControl>& control)
{
	control = std::shared_ptr<MControl>(new MControl(this));
}

GLvoid MApplication::CreateView(std::shared_ptr<MskView>& view)
{
	view = std::shared_ptr<MView>(new MView(this));
}

GLvoid MApplication::CreateImage(std::shared_ptr<MskImage>& image)
{
	image = std::shared_ptr<MImage>(new MImage());
}

GLvoid MApplication::CreateShader(const MskShaderType& type, std::shared_ptr<MskShader>& shader)
{
	shader = std::shared_ptr<MShader>(new MShader(type));
}

GLvoid MApplication::CreateShaderProgram(std::shared_ptr<MskShaderProgram>& shaderProgram)
{
	shaderProgram = std::shared_ptr<MShaderProgram>(new MShaderProgram());
}

GLvoid MApplication::CreateFTFont(std::string filepath, std::shared_ptr<MskFont>& font)
{
	font = std::shared_ptr<MFont>(new MFont(filepath));
}

GLvoid MApplication::CreateTextRenderer(std::shared_ptr<MskTextRenderer>& textRenderer)
{
	textRenderer = std::shared_ptr<MTextRenderer>(new MTextRenderer());
}

GLvoid MApplication::CreateShapeRenderer(std::shared_ptr<MskShapeRenderer>& shapeRenderer)
{
	shapeRenderer = std::shared_ptr<MShapeRenderer>(new MShapeRenderer());
}

GLvoid MApplication::CreateCamera(std::shared_ptr<MskCamera>& mskCamera)
{
	mskCamera = std::shared_ptr<MCamera>(new MCamera());
}

GLvoid MApplication::CreateTexture(std::shared_ptr<MskTexture>& mskTexture)
{
	mskTexture = std::shared_ptr<MTexture>(new MTexture(this));
}

GLvoid MApplication::CreateTextureRenderer(std::shared_ptr<MskTextureRenderer>& mskTextureRenderer)
{
	mskTextureRenderer = std::shared_ptr<MTextureRenderer>(new MTextureRenderer());
}

GLvoid MApplication::CreateFrameRenderer(std::shared_ptr<MskFrameRenderer>& mskFrameRenderer)
{
	mskFrameRenderer = std::shared_ptr<MFrameRenderer>(new MFrameRenderer(this));
}

GLvoid MApplication::CreateMultisampleFrameRenderer(std::shared_ptr<MskMultisampleFrameRenderer>& mskFrameRenderer)
{
	mskFrameRenderer = std::shared_ptr<MMultisampleFrameRenderer>(new MMultisampleFrameRenderer(this));
}

GLvoid MApplication::CreateMesh(std::shared_ptr<MskMesh>& mskMesh)
{
	mskMesh = std::shared_ptr<MMesh>(new MMesh());
}

GLvoid MApplication::CreateCairo(std::shared_ptr<MskCairo>& cairo)
{
	cairo = std::shared_ptr<MCairo>(new MCairo());
}

GLvoid MApplication::CreateLineRenderer(std::shared_ptr<MskLineRenderer>& mskLineRenderer)
{
	mskLineRenderer = std::shared_ptr<MLineRenderer>(new MLineRenderer(this));
}

GLuint MApplication::GetWidth()
{
	return m_nWidth;
}

GLuint MApplication::GetHeight()
{
	return m_nHeight;
}

std::shared_ptr<MskCamera> MApplication::GetCamera()
{
	return mskCamera;
}

void MApplication::OnMouseMove(MskObject* sender, MouseEventArgs* e)
{
	mskCamera->Look(e->x, e->y, e->buttonDown);
}

void MApplication::OnKeyDown(MskObject* sender, KeyEventArgs* e)
{
	if (e->Key == 87) {
		mskCamera->MoveForward(true);
	} else if (e->Key == 83) {
		mskCamera->MoveBackward(true);
	} else if (e->Key == 65) {
		mskCamera->MoveLeft(true);
	} else if (e->Key == 68) {
		mskCamera->MoveRight(true);
	} else if (e->Key == 81) {
		mskCamera->SetMode(1);
		mskCamera->SetViewport(0, 0, m_nWidth, m_nHeight);
		mskCamera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	} else if (e->Key == 69) {
		mskCamera->SetMode(0);
		mskCamera->SetViewport(0, 0, m_nWidth, m_nHeight);
		mskCamera->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void MApplication::OnKeyUp(MskObject* sender, KeyEventArgs* e)
{
	if (e->Key == 87) {
		mskCamera->MoveForward(false);
	}
	else if (e->Key == 83) {
		mskCamera->MoveBackward(false);
	}
	else if (e->Key == 65) {
		mskCamera->MoveLeft(false);
	}
	else if (e->Key == 68) {
		mskCamera->MoveRight(false);
	}
}

void MApplication::OnUpdate(MskObject * sender, UpdateEventArgs * e)
{
	mskCamera->Update(e->DeltaTime);
}

void MApplication::OnLoad(MskObject * sender, EventArgs * e)
{
	// Create font and text renderer
	CreateFTFont("C:\\Windows\\fonts\\arial.ttf", mskFont);
	CreateTextRenderer(mskTextRenderer);

	// Create Shape Renderer
	CreateShapeRenderer(shapeRenderer);

	// Create file menu
	std::shared_ptr<MskScene> fileMenu = std::shared_ptr<MSceneFileMenu>(new MSceneFileMenu(this));

	mskView->AddScene(fileMenu);
	mskView->Update += EventHandler<UpdateEventArgs>(this, &MApplication::OnUpdate);
	mskView->Render += EventHandler<EventArgs>(this, &MApplication::OnRender);
	mskView->Resize += EventHandler<ResizeEventArgs>(this, &MApplication::OnResize);
	mskView->MouseClick  += EventHandler<EventArgs>(this, &MApplication::OnMouseClick);
	mskView->KeyDown += EventHandler<KeyEventArgs>(this, &MApplication::OnKeyDown);
	mskView->KeyUp += EventHandler<KeyEventArgs>(this, &MApplication::OnKeyUp);
	mskView->MouseMove += EventHandler<MouseEventArgs>(this, &MApplication::OnMouseMove);
}

void MApplication::OnMouseClick(MskObject * sender, EventArgs * e)
{
}

void MApplication::OnResize(MskObject * sender, ResizeEventArgs * e)
{
	m_nHeight = e->m_nHeight;
	m_nWidth = e->m_nWidth;
	mskCamera->SetViewport(0, 0, e->m_nWidth, e->m_nHeight);
}

void MApplication::OnRender(MskObject * sender, EventArgs * e)
{
	//mskTextRenderer->Begin();
	//mskTextRenderer->SetProjectionMatrix(mskCamera->GetModelViewProjectionMatrix());
	//mskTextRenderer->RenderText("The quick brown fox jumped over the fence.", 0, 0, 1.0f, mskFont);
	//mskTextRenderer->End();
}

int MApplication::Run()
{
	UpdateEventArgs* updateEventArgs = new UpdateEventArgs();
	EventArgs* args = new EventArgs();

	// Show the view
	mskView->Show();

	// Start timer
	auto startTime = std::chrono::system_clock::now();

	GLfloat frameRate = 60;
	GLfloat frameInterval = 1000 / frameRate;
	GLfloat time = 0;

	MSG msg;
	BOOL done = FALSE;
	while (!done)
	{
		auto endTime = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> dt = endTime - startTime;
		updateEventArgs->DeltaTime = dt.count();

		time += updateEventArgs->DeltaTime;

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
			// If we are not processing messages we give other task time to run
			Idle(this, args);

			// Opengl update and render
			mskView->MakeCurrent();
			mskView->Update(this, updateEventArgs);
			mskView->ClearScreen();
			mskView->Render(this, args);
			mskView->SwapBuffers();
		}

		// Reset time
		startTime = std::chrono::system_clock::now();
	}

	delete updateEventArgs;
	delete args;

	return (int)msg.wParam;
}
