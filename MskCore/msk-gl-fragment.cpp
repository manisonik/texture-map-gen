#include "stdafx.h"
#include "msk-gl-fragment.h"

using namespace Manisonik;

MFragment::MFragment(int x, int y, int width, int height)
{
    m_nX = x;
    m_nY = y;
    m_nWidth = width;
    m_nHeight = height;
}

MFragment::~MFragment()
{
    m_pCamera.reset();
}

void MFragment::Render()
{
    // Set camera viewport 
    m_pCamera->SetViewport(0, 0, m_nWidth, m_nHeight);
    OnRender();
}

std::shared_ptr<MCamera>& MFragment::GetCamera()
{
    return m_pCamera;
}
