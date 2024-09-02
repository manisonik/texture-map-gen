#include "stdafx.h"
#include "msk-gl-textureregion.h"

using namespace TexGen;

TgTextureRegion::TgTextureRegion() :
	m_regionWidth(0),
	m_regionHeight(0)
{}

TgTextureRegion::TgTextureRegion(const TgTexture& texture) :
	m_texture(texture)	
{
	setRegion(0, 0, texture.getTextureData().m_width, texture.getTextureData().m_height);
}

TgTextureRegion::TgTextureRegion(const TgTexture &texture, float width, float height) :
	m_texture(texture)	
{
	setRegion(0, 0, width, height);
}

TgTextureRegion::TgTextureRegion(const TgTexture& texture, float x, float y, float width, float height) :
	m_texture(texture)
{
	setRegion(x, y, width, height);
}

TgTextureRegion::~TgTextureRegion()
{
}

/*void TgTextureRegion::setRegion(float u, float v, float u2, float v2)
{
	int texWidth = m_pTexture->GetTextureData()->m_nWidth;
	int texHeight = m_pTexture->GetTextureData()->m_nHeight;   

	m_nRegionWidth = round(abs(u2 - u) * texWidth);
	m_nRegionHeight = round(abs(v2 - v2) * texHeight);

	m_u = u;
	m_v = v;
	m_u2 = u2;
	m_v2 = v2;
}*/

void TgTextureRegion::setRegion(float x, float y, float width, float height)
{
	float invTexWidth = 1.0f / m_texture.getTextureData().m_width;
	float invTexHeight = 1.0f / m_texture.getTextureData().m_height;

	m_u = x * invTexWidth;
	m_v = y * invTexHeight;
	m_u2 = (x + width) * invTexWidth;
	m_v2 = (y + height) * invTexHeight;

	m_regionWidth = width;
	m_regionHeight = height;
}

void TgTextureRegion::setRegion(const TgTexture& texture)
{
	setRegion(0, 0, texture.getTextureData().m_width, texture.getTextureData().m_height);
}

const TgTexture& TgTextureRegion::getTexture() const
{
	return m_texture;
}

float TgTextureRegion::getU() const
{
	return m_u;
}

float TgTextureRegion::getV() const
{
	return m_v;
}

float TgTextureRegion::getU2() const
{
	return m_u2;
}

float TgTextureRegion::getV2() const
{
	return m_v2;
}
