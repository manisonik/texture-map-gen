#pragma once
#include "msk-core.h"

class MTextureAtlas :
	public MskTextureAtlas
{
private:
	std::vector<std::shared_ptr<MskTextureRegion>> m_pTextureRegion;

public:
	MTextureAtlas();
	~MTextureAtlas();

	GLvoid AddRegion(std::shared_ptr<MskTextureRegion> region);
};

