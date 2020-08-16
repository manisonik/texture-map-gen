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
	m_pMeshCube->SetColor(glm::vec4(0.0f, 1.0f, 1.0f, 0.5f));
	m_pMeshCube->Scale(1.0f, 1.0f, 1.0f);
	m_pMeshCube->Update();
}

void MSceneFileMenu::OnRender(MskObject* sender, EventArgs* e)
{
	// Draw cube
	m_pScene->Begin(m_pCliffDiffuse, m_pCliffNormal, m_pCliffHeight);
	m_pScene->SetCamera(m_pSceneCamera);
	m_pScene->Draw(m_pMeshCube);
	m_pScene->End();

	// Draw the grid
	m_pLineRenderer->Begin();
	m_pLineRenderer->SetProjectionMatrix(m_pSceneCamera->GetModelViewProjectionMatrix());
	if (m_bHitTest) {
		m_pLineRenderer->Draw(m_pMeshCube);
	}
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
	m_pTextRenderer->RenderText(glm::to_string(m_pRay->GetDirection()), -m_nWidth / 2.0f + 500, m_nHeight / 2.0f - 20, 1.0f);
	m_pTextRenderer->End();

	// Start drawing the buttons
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

void MSceneFileMenu::OnKeyUp(MskObject* sender, KeyEventArgs* e)
{
	if (e->Key == 73) {
		m_pScene->EnablePOM(false);
	} else if (e->Key == 79) {
		m_pScene->EnablePOM(true);
	}
}

void MSceneFileMenu::OnMouseMove(MskObject* sender, MouseEventArgs* e)
{
	m_pRay = m_pSceneCamera->GetRay(e->x, e->y);

	// Loop through meshes
	std::vector<MskFace> faces = m_pMeshCube->GetFaces();
	std::vector<glm::vec3> vertices = m_pMeshCube->GetVertices();

	m_bHitTest = 0;
	for (auto it = faces.begin(); it != faces.end(); ++it) {
		if (it->indices.size() % 3 == 0) {
			glm::vec3 v0 = glm::vec3(glm::vec4(vertices[it->indices[0]], 1.0f) * m_pMeshCube->GetModelMatrix());
			glm::vec3 v1 = glm::vec3(glm::vec4(vertices[it->indices[1]], 1.0f) * m_pMeshCube->GetModelMatrix());
			glm::vec3 v2 = glm::vec3(glm::vec4(vertices[it->indices[2]], 1.0f) * m_pMeshCube->GetModelMatrix());

			glm::vec3 v3 = glm::vec3(glm::vec4(vertices[it->indices[3]], 1.0f) * m_pMeshCube->GetModelMatrix());
			glm::vec3 v4 = glm::vec3(glm::vec4(vertices[it->indices[4]], 1.0f) * m_pMeshCube->GetModelMatrix());
			glm::vec3 v5 = glm::vec3(glm::vec4(vertices[it->indices[5]], 1.0f) * m_pMeshCube->GetModelMatrix());

			glm::vec3 bary;
			if (m_pRay->Intersect(v0, v1, v2, bary) || m_pRay->Intersect(v3, v4, v5, bary)) {
				m_bHitTest++;
			}
		}
	}
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
	mskApp->CreateTexture(m_pCliffDiffuse);
	mskApp->CreateTexture(m_pCliffNormal);
	mskApp->CreateTexture(m_pCliffHeight);
	mskApp->CreateCairo(m_pCairo);

	// Add texture
	m_pCliffDiffuse->SetFilter(MskTextureFilter::MipMapLinearLinear, MskTextureFilter::MipMapLinearLinear);
	m_pCliffDiffuse->LoadFromFile("cliff02.png");

	m_pCliffNormal->SetFilter(MskTextureFilter::MipMapLinearLinear, MskTextureFilter::MipMapLinearLinear);
	m_pCliffNormal->LoadFromFile("cliff02_normal.png");

	m_pCliffHeight->SetFilter(MskTextureFilter::MipMapLinearLinear, MskTextureFilter::MipMapLinearLinear);
	m_pCliffHeight->LoadFromFile("cliff02_disp.png");

	// Store camera
	m_pSceneCamera = mskApp->GetCamera();

	// Get ray
	m_pRay = m_pSceneCamera->GetRay(0.0f, 0.0f);

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
	Render += EventHandler(this, &MSceneFileMenu::OnRender);
	Resize += EventHandler(this, &MSceneFileMenu::OnResize);
	Update += EventHandler(this, &MSceneFileMenu::OnUpdate);
	MouseMove += EventHandler(this, &MSceneFileMenu::OnMouseMove);
	KeyUp += EventHandler(this, &MSceneFileMenu::OnKeyUp);
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
