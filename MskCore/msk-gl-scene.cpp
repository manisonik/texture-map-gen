#include "stdafx.h"
#include "msk-gl-scene.h"

using namespace Manisonik;

MScene::MScene(int x, int y, int width, int height)
{
	m_nX = x;
	m_nY = y;
	m_nWidth = width;
	m_nHeight = height;
}

MScene::~MScene()
{
}

void MScene::Render()
{
	// Set camera viewport
	m_pSceneCamera->SetViewport(m_nX, m_nY, m_nWidth, m_nHeight);

	// Render main scene
	OnRender();

	// Render fragments
	for (auto& frag : m_pFragmentList)
	{
		frag->Render();
	}
}

void MScene::Update(float dt)
{
	m_pSceneCamera->Update(dt);
	OnUpdate(dt);
}

std::shared_ptr<MCamera>& MScene::GetCamera()
{
	return m_pSceneCamera;
}

void MScene::AddFragment(const std::shared_ptr<MFragment>& fragment)
{
	m_pFragmentList.push_back(fragment);
}

void MScene::Initialize()
{
	// Setup camera for scene
	m_pSceneCamera = std::make_shared<MCamera>();
	m_pSceneCamera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));

	OnInitialize();
}
