#pragma once
#include "msk-core.h"

class MTexture : public MskTexture
{
private:
	GLuint m_nTextureId;
	MskTextureTarget m_nTarget;
	MskTextureFilter m_nMagFilter;
	MskTextureFilter m_nMinFilter;
	MskTextureWrap m_nWrapU;
	MskTextureWrap m_nWrapV;
	MskTextureFormat m_nInternalFormat;
	MskTextureFormat m_nFormat;
	ILuint m_pImage;
	std::shared_ptr<MskTextureData> m_pImageData;
	MskApp* m_pMskApp;
	GLboolean m_bMultisampling;
	GLuint m_nSamples;

public:
	MTexture(MskApp * app);
	~MTexture();

	GLvoid Bind(GLuint unit);
	GLvoid Bind();
	GLvoid SetInternalFormat(MskTextureFormat internalFormat);
	GLvoid SetFormat(MskTextureFormat format);
	GLvoid SetFilter(MskTextureFilter minFilter, MskTextureFilter magFilter);
	GLvoid SetWrap(MskTextureWrap u, MskTextureWrap v);
	GLuint GetTextureId();
	GLboolean LoadFromFile(std::string filename);
	GLboolean LoadFromMemory(std::shared_ptr<MskTextureData> textureData);
	MskTextureTarget GetTextureTarget();
	GLvoid EnableMultisampling(GLuint samples);
private:
	void GenerateMipMaps();
};

