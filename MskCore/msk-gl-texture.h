#pragma once
#include "msk-enum.h"
#include "msk-gl-shaderprogram.h"
#include "msk-gl-texturedata.h"

namespace Manisonik {
	class MTexture
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
		std::shared_ptr<MTextureData> m_pImageData;
		GLboolean m_bMultisampling;
		GLuint m_nSamples;
		GLfloat m_nWidth;
		GLfloat m_nHeight;

	public:
		MTexture();
		~MTexture();

		std::shared_ptr<MTextureData>& GetTextureData();
		GLvoid Bind(GLuint unit);
		GLvoid Bind();
		GLvoid SetInternalFormat(MskTextureFormat internalFormat);
		GLvoid SetFormat(MskTextureFormat format);
		GLvoid SetFilter(MskTextureFilter minFilter, MskTextureFilter magFilter);
		GLvoid SetWrap(MskTextureWrap u, MskTextureWrap v);
		GLuint GetTextureId();
		GLboolean LoadFromFile(std::string filename);
		GLboolean LoadFromMemory(std::shared_ptr<MTextureData> textureData);
		MskTextureTarget GetTextureTarget();
		GLvoid EnableMultisampling(GLuint samples);
	private:
		void GenerateMipMaps();
	};
}

