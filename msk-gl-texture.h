#pragma once
#include "msk-enum.h"
#include "msk-gl-shaderprogram.h"
#include "msk-gl-texturedata.h"

namespace TexGen {
	class TgTexture
	{
	private:
		unsigned int m_textureId;
		eTextureTarget m_target;
		eTextureFilter m_magFilter;
		eTextureFilter m_minFilter;
		eTextureWrap m_wrapU;
		eTextureWrap m_wrapV;
		eTextureFormat m_internalFormat;
		eTextureFormat m_format;
		TgTextureData m_imageData;
		unsigned int m_image;
		bool m_multisampling;
		unsigned int m_samples;
		float m_width;
		float m_height;

	public:
		TgTexture();
		~TgTexture();

		const TgTextureData& getTextureData() const;
		void bind(unsigned int unit);
		void bind() const;
		void setInternalFormat(eTextureFormat internalFormat);
		void setFormat(eTextureFormat format);
		void setFilter(eTextureFilter minFilter, eTextureFilter magFilter);
		void setWrap(eTextureWrap u, eTextureWrap v);
		unsigned int getTextureId() const;
		bool loadFromFile(const std::string& filename);
		bool loadFromMemory(const TgTextureData& textureData);
		eTextureTarget getTextureTarget() const;
		void enableMultisampling(unsigned int samples);
	private:
		void generateMipMaps();
	};
}

