#include "stdafx.h"
#include "msk-gl-main-scene.h"

using namespace Manisonik;

MMainScene::MMainScene(int x, int y, int width, int height) 
    : MScene(x, y, width, height)
{
    // Bind stencil functions
    m_pStencilFunc = std::bind(&MMainScene::RenderPass, this, std::placeholders::_1);
}

MMainScene::~MMainScene()
{
    m_pRenderer.reset();
    m_pLineRenderer.reset();
    m_pSceneCamera.reset();
}

void MMainScene::OnResize(MObject* sender, MResizeEventArgs* e)
{
    m_nWidth = e->width;
    m_nHeight = e->height;
}

void MMainScene::OnMouseUp(MObject* sender, MMouseEventArgs* e)
{
}

void MMainScene::OnMouseOver(MObject* sender, MMouseEventArgs* e)
{
    if (e->button == MouseButton::ButtonLeft)
        GetCamera()->Look(e->x, e->y, true);
    else
        GetCamera()->Look(e->x, e->y, false);
}

void MMainScene::OnMouseDown(MObject* sender, MMouseEventArgs* e)
{
}

void MMainScene::OnMouseClick(MObject* sender, MMouseEventArgs* e)
{
}

void MMainScene::OnMouseMove(MObject* sender, MMouseEventArgs* e)
{
}

void MMainScene::OnKeyUp(MObject* sender, MKeyEventArgs* e)
{
    switch (e->KeyCode)
    {
    case Keys::W: // W
        GetCamera()->MoveForward(false);
        break;
    case Keys::S: // S
        GetCamera()->MoveBackward(false);
        break;
    case Keys::A: // A
        GetCamera()->MoveLeft(false);
        break;
    case Keys::D: // D
        GetCamera()->MoveRight(false);
        break;
    }
}

void MMainScene::OnKeyDown(MObject* sender, MKeyEventArgs* e)
{
    switch (e->KeyCode)
    {
    case Keys::W: // W
        GetCamera()->MoveForward(true);
        break;
    case Keys::S: // S
        GetCamera()->MoveBackward(true);
        break;
    case Keys::A: // A
        GetCamera()->MoveLeft(true);
        break;
    case Keys::D: // D
        GetCamera()->MoveRight(true);
        break;
    }
}

void MMainScene::OnRender()
{
    // Set camera viewport
    //m_pSceneCamera->SetViewport(300, 0, m_nWidth - 300, m_nHeight);

    m_pStencilRenderer->Begin();
    m_pStencilRenderer->Draw(m_pStencilFunc);
    m_pStencilRenderer->End();

    // Set camera viewport 
    /*m_pCameraUI->SetViewport(0, 0, m_nWidth, m_nHeight);

    m_pSpriteBatch->SetProjectionMatrix(m_pCameraUI->GetModelViewProjectionMatrix());
    m_pSpriteBatch->Begin();
    m_pSpriteBatch->Draw(m_pTexRgn[0], -m_nWidth / 2.0f + 300, 0, 32, 32);
    m_pSpriteBatch->Draw(m_pTexRgn[1], -m_nWidth / 2.0f + 300, 34, 32, 32);
    m_pSpriteBatch->Draw(m_pTexRgn[2], -m_nWidth / 2.0f + 300, 68, 32, 32);
    //m_pSpriteBatch->Draw(m_pTexRgn[3], -m_nWidth / 2.0f + 100, 102, 108, 32);
    m_pSpriteBatch->End();*/
}

void MMainScene::OnUpdate(float dt)
{
    //m_pSceneCamera->Update(dt);
    //m_pCameraUI->Update(dt);
    m_pTextRenderer->Update(dt);
}

void MMainScene::RenderPass(int pass)
{
    if (pass == 0)
    {
        m_pRenderer->SetProjectionMatrix(GetCamera()->GetModelViewProjectionMatrix());
        m_pRenderer->Draw();
    }
    else
    {
        // Increase size
        /*m_pMeshCube->Scale(1.01f, 1.01f, 1.01f);
        m_pMeshCube->Update();

        m_pRenderer->Begin();
        m_pRenderer->SetProjectionMatrix(m_pSceneCamera->GetModelViewProjectionMatrix());
        m_pRenderer->Draw(m_pMeshCube);
        m_pRenderer->End();

        // Set back to normal
        m_pMeshCube->Scale(1.0f, 1.0f, 1.0f);
        m_pMeshCube->Update();*/
    }
}

void MMainScene::OnInitialize()
{
    // Register events
    ResizeEvent += MEventHandler<MResizeEventArgs>(this, &MMainScene::OnResize);
    MouseMoveEvent += MEventHandler<MMouseEventArgs>(this, &MMainScene::OnMouseOver);
    KeyUpEvent += MEventHandler<MKeyEventArgs>(this, &MMainScene::OnKeyUp);
    KeyDownEvent += MEventHandler<MKeyEventArgs>(this, &MMainScene::OnKeyDown);

    // Create stencil renderer
    m_pStencilRenderer = std::make_shared<MStencilRenderer>();

    // Create cube
    std::unique_ptr<MMeshBuilder> builder = std::make_unique<MMeshBuilder>();
    builder->CreateGrid(m_pGrid);
    builder->CreateCylinder(m_pMeshCylinder);
    builder->CreateSphere(m_pMeshSphere);
    builder->CreateIcoSphere(m_pMeshIcoSphere);


    // Create renderer
    m_pRenderer = std::make_shared<MRenderer>();
    //m_pRenderer->AddMesh(m_pMeshCylinder);
    m_pRenderer->AddMesh(m_pMeshSphere);
    //m_pRenderer->AddMesh(m_pMeshIcoSphere);
    m_pRenderer->AddMesh(m_pGrid); 

    // Create line renderer
    m_pLineRenderer = std::make_shared<MLineRenderer>();
    
    // Setup camera for UI
    m_pCameraUI = std::make_shared<MCamera>();
    m_pCameraUI->SetMode(0);

    // Create Mouse Picker
    m_pMousePicker = std::make_shared<MMousePicker>();
    m_pMousePicker->AddMesh(m_pMeshCube);

    // Create font
    m_pFont = std::make_shared<MFont>("C:\\Windows\\fonts\\arial.ttf");

    // Create text renderer
    m_pTextRenderer = std::make_shared<MTextRenderer>();

    // Create sprite batch 
    m_pSpriteBatch = std::make_shared<MSpriteBatch>();

    // Create texture
    m_pTexture = std::make_shared<MTexture>();
    m_pTexture->LoadFromFile("ui_icons.png");

    // Create texture region
    m_pTexRgn.push_back(std::make_shared<MTextureRegion>(m_pTexture, 0, 0, 32, 32));
    m_pTexRgn.push_back(std::make_shared<MTextureRegion>(m_pTexture, 38, 0, 32, 32));
    m_pTexRgn.push_back(std::make_shared<MTextureRegion>(m_pTexture, 74, 0, 32, 32));
    m_pTexRgn.push_back(std::make_shared<MTextureRegion>(m_pTexture, 228, 0, 108, 32));
}