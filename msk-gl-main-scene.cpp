#include "stdafx.h"
#include "msk-gl-main-scene.h"

using namespace TexGen;

TgMainScene::TgMainScene(int x, int y, int width, int height) : 
    TgScene(x, y, width, height),
    m_font("C:\\Windows\\fonts\\arial.ttf"),
    m_stencilFunc(std::bind(&TgMainScene::renderPass, this, std::placeholders::_1)),
    m_ray(glm::vec3(0,0,0))
{
    // Create cube
    TgMeshBuilder builder;
    builder.createSphere();

    auto meshCube = builder.getMesh();
    meshCube.scale(2.0, 1.0, 1.0);
    meshCube.update();
    m_renderer.addMesh(meshCube);
    
    // Setup camera for UI
    m_cameraUI.setMode(0);

    // Create Mouse Picker
    //m_mousePicker.addMesh(m_meshCube);

	GLenum error = glGetError();

    m_texture.loadFromFile("C:/Source/TextureMapGen/out/build/default/ui_icons.png");

    // Create texture region
    m_texRgn.push_back(std::make_shared<TgTextureRegion>(m_texture, 0, 0, 32, 32));
    m_texRgn.push_back(std::make_shared<TgTextureRegion>(m_texture, 38, 0, 32, 32));
    m_texRgn.push_back(std::make_shared<TgTextureRegion>(m_texture, 74, 0, 32, 32));
    m_texRgn.push_back(std::make_shared<TgTextureRegion>(m_texture, 228, 0, 108, 32));
}

TgMainScene::~TgMainScene()
{
}

void TgMainScene::onResize(int32_t width, int32_t height)
{
    m_width = width;
    m_height = height;
}

void TgMainScene::onMouseUp(int x, int y, eMouseButton button)
{
}

void TgMainScene::onMouseOver(int x, int y, eMouseButton button)
{
    if (button == eMouseButton::ButtonLeft)
        getCamera().look(x, y, true);
    else
        getCamera().look(x, y, false);
}

void TgMainScene::onMouseDown(int x, int y, eMouseButton button)
{
}

void TgMainScene::onMouseClick(int x, int y, eMouseButton button)
{
}

void TgMainScene::onMouseMove(int x, int y, eMouseButton button)
{
}

void TgMainScene::onKeyUp(eKeys keyCode)
{
    switch (keyCode)
    {
    case eKeys::W: // W
        getCamera().moveForward(false);
        break;
    case eKeys::S: // S
        getCamera().moveBackward(false);
        break;
    case eKeys::A: // A
        getCamera().moveLeft(false);
        break;
    case eKeys::D: // D
        getCamera().moveRight(false);
        break;
    }
}

void TgMainScene::onKeyDown(eKeys keyCode)
{
    switch (keyCode)
    {
    case eKeys::W: // W
        getCamera().moveForward(true);
        break;
    case eKeys::S: // S
        getCamera().moveBackward(true);
        break;
    case eKeys::A: // A
        getCamera().moveLeft(true);
        break;
    case eKeys::D: // D
        getCamera().moveRight(true);
        break;
    }
}

void TgMainScene::onRender()
{
    // Set camera viewport
    m_sceneCamera.setViewport(300, 0, m_width - 300, m_height);

    m_stencilRenderer.begin();
    m_stencilRenderer.draw(m_stencilFunc);
    m_stencilRenderer.end();

    // Set camera viewport 
    m_cameraUI.setViewport(0, 0, m_width, m_height);

    m_spriteBatch.setProjectionMatrix(m_cameraUI.getModelViewProjectionMatrix());
    m_spriteBatch.begin();
    m_spriteBatch.draw(*m_texRgn[0], -m_width / 2.0f + 300, 0, 32, 32);
    m_spriteBatch.draw(*m_texRgn[1], -m_width / 2.0f + 300, 34, 32, 32);
    m_spriteBatch.draw(*m_texRgn[2], -m_width / 2.0f + 300, 68, 32, 32);
    m_spriteBatch.draw(*m_texRgn[3], -m_width / 2.0f + 100, 102, 108, 32);
    m_spriteBatch.end();
}

void TgMainScene::onUpdate(float dt)
{
    //m_sceneCamera.update(dt);
    m_cameraUI.update(dt);
    //m_textRenderer.update(dt);
}

void TgMainScene::renderPass(int pass)
{
    if (pass == 0)
    {
        m_renderer.setProjectionMatrix(getCamera().getModelViewProjectionMatrix());
        m_renderer.draw();
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