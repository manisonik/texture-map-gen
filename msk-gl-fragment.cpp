#include "stdafx.h"
#include "msk-gl-fragment.h"

using namespace TexGen;

TgFragment::TgFragment(int x, int y, int width, int height) :
    m_x(x),
    m_y(y),
    m_width(width),
    m_height(height)
{
}

TgFragment::~TgFragment()
{
}

void TgFragment::render()
{
    // Set camera viewport 
    //m_camera.setViewport(0, 0, m_width, m_height);
    //onRender();
}

const TgCamera& TgFragment::getCamera() const
{
    return m_camera;
}
