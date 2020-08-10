#include "stdafx.h"
#include "msk-gl-scene-filemenu.h"

GLvoid MSceneFileMenu::InitMesh()
{
	// Start adding fragments for UI
	m_pMeshFileMenu->AddPlane();
	m_pMeshFileMenu->SetColor(glm::vec4(0.18f, 0.18f, 0.18f, 1.0f));
	m_pMeshSideWindow->AddPlane();
	m_pMeshSideWindow->SetColor(glm::vec4(0.18f, 0.18f, 0.18f, 1.0f));

	// Setup grid
	m_pMeshGrid->SetColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	m_pMeshGrid->AddGrid(50, 50, 50);

	// Resize cube
	m_pMeshCube->AddCube();
	m_pMeshCube->Scale(1.0f, 1.0f, 1.0f);
	m_pMeshCube->Update();
}

void MSceneFileMenu::OnRender(MskObject* sender, EventArgs* e)
{
	// Draw cube
	m_pScene->Begin(m_pCliffTexture);
	m_pScene->SetCamera(m_pSceneCamera);
	m_pScene->Draw(m_pMeshCube);
	m_pScene->End();

	// Draw the grid
	m_pLineRenderer->Begin();
	m_pLineRenderer->SetProjectionMatrix(m_pSceneCamera->GetModelViewProjectionMatrix());
	m_pLineRenderer->Draw(m_pMeshGrid);
	m_pLineRenderer->End();

	// Begin rendering UI
	m_pShapeRenderer->Begin();
	m_pShapeRenderer->SetProjectionMatrix(m_pCamera->GetModelViewProjectionMatrix());
	m_pShapeRenderer->Draw(m_pMeshFileMenu);
	m_pShapeRenderer->Draw(m_pMeshSideWindow);
	m_pShapeRenderer->End();

	m_pTextRenderer->Begin(m_pFont);
	m_pTextRenderer->SetProjectionMatrix(m_pCamera->GetModelViewProjectionMatrix());
	m_pTextRenderer->RenderText("File    Edit    View    Project    Build", -m_nWidth / 2.0f + 10, m_nHeight / 2.0f - 20, 1.0f);
	m_pTextRenderer->End();

	m_pTextureRenderer->Begin(m_pTexture);
	m_pTextureRenderer->SetCamera(m_pCamera);
	for (auto it = m_vButtons.begin(); it != m_vButtons.end(); ++it) {
		m_pTextureRenderer->Draw(*it);
	}
	m_pTextureRenderer->End();
}

void MSceneFileMenu::OnResize(MskObject* sender, ResizeEventArgs* e)
{
	// Set the viewport for the UI
	m_pCamera->SetViewport(0, 0, e->m_nWidth, e->m_nHeight);

	// Scale and translate the file menu into position
	m_pMeshFileMenu->Scale(e->m_nWidth/2, 14, 1.0f);
	m_pMeshFileMenu->Translate(0.0f, e->m_nHeight/2 - 14, 0.0f);
	m_pMeshFileMenu->Update();

	// Scale and translate the sidebar into position
	m_pMeshSideWindow->Scale(200, e->m_nHeight / 2, 1.0f);
	m_pMeshSideWindow->Translate(e->m_nWidth/-2.0f + 100, 0.0, 0.0f);
	m_pMeshSideWindow->Update();

	// Resize the buttons
	for (int i = 0; i < 4; i++)
	{
		m_vButtons[i]->Translate(e->m_nWidth / -2.0f + 330, i * 55.0f, 0.0f);
		m_vButtons[i]->Update();
	}

	// Store width and height
	m_nWidth = e->m_nWidth;
	m_nHeight = e->m_nHeight;
}

void MSceneFileMenu::OnUpdate(MskObject* sender, UpdateEventArgs* e)
{
	m_pCamera->Update(e->DeltaTime);
}

void MSceneFileMenu::OnMouseMove(MskObject* sender, MouseEventArgs* e)
{
	std::shared_ptr<MskRay> ray = m_pCamera->GetRay(e->x, e->y);
}

MSceneFileMenu::MSceneFileMenu(MskApp* mskApp)
{
	mskApp->CreateCamera(m_pCamera);
	mskApp->CreateShapeRenderer(m_pShapeRenderer);
	mskApp->CreateMesh(m_pMeshFileMenu);
	mskApp->CreateMesh(m_pMeshSideWindow);
	mskApp->CreateMesh(m_pMeshGrid);
	mskApp->CreateMesh(m_pMeshCube);
	mskApp->CreateLineRenderer(m_pLineRenderer);
	mskApp->CreateTextureRenderer(m_pTextureRenderer);
	mskApp->CreateTextureRenderer(m_pScene);
	mskApp->CreateFTFont("C:\\Windows\\fonts\\arial.ttf", m_pFont);
	mskApp->CreateTextRenderer(m_pTextRenderer);
	mskApp->CreateTexture(m_pTexture);
	mskApp->CreateTexture(m_pCliffTexture);
	mskApp->CreateCairo(m_pCairo);

	// Add texture
	m_pCliffTexture->SetFilter(MskTextureFilter::MipMapLinearLinear, MskTextureFilter::MipMapLinearLinear);
	m_pCliffTexture->LoadFromFile("cliff02.png");

	// Store camera
	m_pSceneCamera = mskApp->GetCamera();

	// Enable lighting
	m_pScene->EnableLighting(true);

	// Create buttons
	for (int i = 0; i < 4; i++) {
		std::shared_ptr<MskMesh> button;
		mskApp->CreateMesh(button);
		button->AddPlane();
		button->Scale(25.0f, 25.0f, 1.0f);
		button->Update();
		m_vButtons.push_back(button);
	}

	// Create dummy texture
	m_pCairo->Clear(0.3, 0.3, 0.3, 0.7);

	std::shared_ptr<MskTextureData> textureData = std::shared_ptr<MskTextureData>(new MskTextureData());
	textureData->m_pBytes = m_pCairo->GetSurfaceData();
	textureData->m_nColorType = GL_RGBA;
	textureData->m_nFormat = GL_RGBA;
	textureData->m_nNumOfPasses = 1;
	textureData->m_nWidth = 100;
	textureData->m_nHeight = 100;
	m_pTexture->LoadFromMemory(textureData);

	// Initalize mesh
	InitMesh();

	// Initalize camera
	m_pCamera->SetMode(0);

	// Register events
	Render += EventHandler<EventArgs>(this, &MSceneFileMenu::OnRender);
	Resize += EventHandler<ResizeEventArgs>(this, &MSceneFileMenu::OnResize);
	Update += EventHandler<UpdateEventArgs>(this, &MSceneFileMenu::OnUpdate);
	MouseMove += EventHandler<MouseEventArgs>(this, &MSceneFileMenu::OnMouseMove);
}

MSceneFileMenu::~MSceneFileMenu()
{
	m_pFont.reset();
	m_pMeshSideWindow.reset();
	m_pMeshFileMenu.reset();
	m_pLineRenderer.reset();
	m_pMeshGrid.reset();
	m_pCamera.reset();
	m_pShapeRenderer.reset();
}
