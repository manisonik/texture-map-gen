#include "stdafx.h"
#include "msk-gui-control.h"

MControl::MControl(MskApp* mskApp)
{
	// Create font and text renderer
	mskApp->CreateFTFont("C:\\Windows\\fonts\\arial.ttf", mskFont);
	mskApp->CreateTextRenderer(mskTextRenderer);
	mskApp->CreateShapeRenderer(shapeRenderer);
	mskApp->CreateTextureRenderer(textureRenderer);
	mskApp->CreateCamera(mskCamera);
	//textureRenderer->CreateCube(meshCube);

	mskApp->CreateTexture(mskTexture);
	mskTexture->LoadFromFile("ground.jpg");
	
	mskCamera->SetMode(0);
	mskCamera->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	mskCamera->SetViewport(0, 0, 800, 600);
	mskCamera->Update(0.0f);

	// Register events
	Render += EventHandler<EventArgs>(this, &MControl::OnRender);
	Resize += EventHandler<ResizeEventArgs>(this, &MControl::OnResize);
	Update += EventHandler<UpdateEventArgs>(this, &MControl::OnUpdate);
}

MControl::~MControl()
{
}

void MControl::OnUpdate(MskObject* sender, UpdateEventArgs* e)
{
	mskCamera->Update(e->DeltaTime);
}

void MControl::OnResize(MskObject* sender, ResizeEventArgs* e) {
	mskCamera->SetViewport(0, 0, e->m_nWidth, e->m_nHeight);
}

void MControl::OnRender(MskObject* sender, EventArgs* e)
{
	//mskTextRenderer->Begin();
	//mskTextRenderer->SetProjectionMatrix(mskCamera->GetModelViewProjectionMatrix());
	//mskTextRenderer->RenderText("The quick brown fox jumped over the fence.", 1, 1, 1.0f, mskFont);
	//mskTextRenderer->End();

	textureRenderer->Begin(mskTexture);
	textureRenderer->SetCamera(mskCamera);
	textureRenderer->Draw(meshCube);
	textureRenderer->End();
}
