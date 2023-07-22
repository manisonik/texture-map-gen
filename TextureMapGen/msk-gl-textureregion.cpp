#include "..\MskCore\msk-gl-textureregion.h"
#include "stdafx.h"
#include "msk-gl-textureregion.h"

MTextureRegion::MTextureRegion(MskTextureAtlas* textureAtlas) :
	u(0), v(0), u2(1), v2(1)
{
}

MTextureRegion::~MTextureRegion()
{
}

std::shared_ptr<MskTexture>& MTextureRegion::GetTexture()
{
	return m_pTexture;
}

GLvoid MTextureRegion::SetRegion(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	u = x;
	v = y;
	u2 = width;
	v2 = height;
}

GLvoid MTextureRegion::Flip(GLboolean x, GLboolean y)
{
}

GLfloat MTextureRegion::GetU()
{
	return u;
}

GLfloat MTextureRegion::GetV()
{
	return v;
}

GLfloat MTextureRegion::GetU2()
{
	return u2;
}

GLfloat MTextureRegion::GetV2()
{
	return v2;
}

GLvoid MTextureRegion::Scroll(GLfloat x, GLfloat y)
{
	return GLvoid();
}
