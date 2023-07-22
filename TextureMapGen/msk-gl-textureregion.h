#pragma once
#include "msk-core.h"

class MTextureRegion :
	public MskTextureRegion
{
private:
	GLfloat u, v, u2, v2;
	std::shared_ptr<MskTexture> m_pTexture;

public:
	MTextureRegion(MskTextureAtlas* app);
	~MTextureRegion();

	std::shared_ptr<MskTexture>& GetTexture();
	GLvoid SetRegion(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	GLvoid Flip(GLboolean x, GLboolean y);
	GLfloat GetU();
	GLfloat GetV();
	GLfloat GetU2();
	GLfloat GetV2();
	GLvoid Scroll(GLfloat x, GLfloat y);
};

