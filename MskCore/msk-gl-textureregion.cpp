#include "stdafx.h"
#include "msk-gl-textureregion.h"

using namespace Manisonik;

MTextureRegion::MTextureRegion()
{
	m_nRegionWidth = 0;
	m_nRegionHeight = 0;
}

MTextureRegion::MTextureRegion(std::shared_ptr<MTexture>& texture)
{
	m_pTexture = texture;
	SetRegion(0, 0, texture->GetTextureData()->m_nWidth, texture->GetTextureData()->m_nHeight);
}

/*MTextureRegion::MTextureRegion(std::shared_ptr<MTexture>& texture, float u, float v, float u2, float v2)
{
	m_pTexture = texture;
	SetRegion(u, v, u2, v2);
}*/

MTextureRegion::MTextureRegion(std::shared_ptr<MTexture>& texture, float x, float y, float width, float height)
{
	m_pTexture = texture;
	SetRegion(x, y, width, height);
}

MTextureRegion::~MTextureRegion()
{
	m_pTexture.reset();
}

/*void MTextureRegion::SetRegion(float u, float v, float u2, float v2)
{
	int texWidth = m_pTexture->GetTextureData()->m_nWidth;
	int texHeight = m_pTexture->GetTextureData()->m_nHeight;   

	m_nRegionWidth = round(abs(u2 - u) * texWidth);
	m_nRegionHeight = round(abs(v2 - v2) * texHeight);

	m_u = u;
	m_v = v;
	m_u2 = u2;
	m_v2 = v2;
} */

void MTextureRegion::SetRegion(float x, float y, float width, float height)
{
	float invTexWidth = 1.0f / m_pTexture->GetTextureData()->m_nWidth;
	float invTexHeight = 1.0f / m_pTexture->GetTextureData()->m_nHeight;

	m_u = x * invTexWidth;
	m_v = y * invTexHeight;
	m_u2 = (x + width) * invTexWidth;
	m_v2 = (y + height) * invTexHeight;

	m_nRegionWidth = width;
	m_nRegionHeight = height;
}

void MTextureRegion::SetRegion(std::shared_ptr<MTexture> texture)
{
	this->m_pTexture = texture;
	SetRegion(0, 0, texture->GetTextureData()->m_nWidth, texture->GetTextureData()->m_nHeight);
}

std::shared_ptr<MTexture>& MTextureRegion::GetTexture()
{
	return m_pTexture;
}

float MTextureRegion::GetU()
{
	return m_u;
}

float MTextureRegion::GetV()
{
	return m_v;
}

float MTextureRegion::GetU2()
{
	return m_u2;
}

float MTextureRegion::GetV2()
{
	return m_v2;
}
