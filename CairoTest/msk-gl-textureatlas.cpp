#include "stdafx.h"
#include "msk-gl-textureatlas.h"

MTextureAtlas::MTextureAtlas()
{
}

MTextureAtlas::~MTextureAtlas()
{
}

GLvoid MTextureAtlas::AddRegion(std::shared_ptr<MskTextureRegion> region)
{
	m_pTextureRegion.push_back(region);
}
