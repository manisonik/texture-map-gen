#include "stdafx.h"
#include "msk-gl-scene.h"

using namespace TexGen;

TgScene::TgScene(int x, int y, int width, int height) :
	m_x(x),
	m_y(y),
	m_width(width),
	m_height(height)
{
	m_sceneCamera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
}

TgScene::~TgScene()
{
}

void TgScene::render()
{
	// Set camera viewport
	m_sceneCamera.setViewport(m_x, m_y, m_width, m_height);

	// Render main scene
	onRender();

	// Render fragments
	for (auto& frag : m_fragmentList)
	{
		frag->render();
	}
}

void TgScene::update(float dt)
{
	m_sceneCamera.update(dt);
	onUpdate(dt);
}

TgCamera& TgScene::getCamera()
{
	return m_sceneCamera;
}

void TgScene::addFragment(std::shared_ptr<TgFragment> fragment)
{
	m_fragmentList.push_back(fragment);
}