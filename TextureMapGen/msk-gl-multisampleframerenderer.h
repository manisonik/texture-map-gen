#pragma once
#include "msk-core.h"

class MMultisampleFrameRenderer :
	public MskMultisampleFrameRenderer
{
private:
	// Shaders
	std::shared_ptr<MskShader> m_pVertexShader;
	std::shared_ptr<MskShader> m_pFragmentShader;
	std::shared_ptr<MskShaderProgram> m_pShaderProgram;
	std::shared_ptr<MskTexture> m_pTexture;
	GLuint m_hRbo;
	GLuint m_hFboColor;
	GLuint m_hFboMSAA;
	GLuint m_hFboDepth;
	GLuint m_hFbo;
	GLuint m_hDepthBuffer;
	GLint m_nMaxSamples = 0;
	GLsizei m_nWidth = 1024;
	GLsizei m_nHeight = 1024;
	GLsizei m_nSamples = 4;

	void InitTextures(MskApp* mskApp);
	void InitFrameBuffer(MskApp* mskApp);
	void InitShaders();

public:
	MMultisampleFrameRenderer(MskApp* mskApp);
	virtual ~MMultisampleFrameRenderer();

	GLvoid Begin();
	GLvoid End();
	GLvoid Resize(GLsizei width, GLsizei height);
	std::shared_ptr<MskTexture> GetTexture();
};

