#pragma once
#include "msk-core.h"

class MFrameRenderer : public MskFrameRenderer
{
private:
	// Shaders
	std::shared_ptr<MskShader> m_pVertexShader;
	std::shared_ptr<MskShader> m_pFragmentShader;
	std::shared_ptr<MskShaderProgram> m_pShaderProgram;
	std::shared_ptr<MskTexture> m_pTexture;
	GLuint m_hDepthBuffer;
	GLuint m_hFrameBuffer;

	void InitFrameBuffer(MskApp* mskApp);
	void InitShaders();
public:
	MFrameRenderer(MskApp* mskApp);
	virtual ~MFrameRenderer();

	GLvoid Begin();
	GLvoid End();
	std::shared_ptr<MskTexture> GetTexture();
};

