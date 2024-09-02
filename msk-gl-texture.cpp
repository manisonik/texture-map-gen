#include "stdafx.h"
#include "msk-enum.h"
#include "msk-gl-texture.h"

using namespace TexGen;

TgTexture::TgTexture() :
	m_minFilter(eTextureFilter::Nearest),
	m_magFilter(eTextureFilter::Nearest),
	m_wrapU(eTextureWrap::ClampToEdge),
	m_wrapV(eTextureWrap::ClampToEdge),
	m_internalFormat(eTextureFormat::RGBA),
	m_format(eTextureFormat::BGRA),
	m_target(eTextureTarget::TwoDimension),
	m_multisampling(false)
{
	ilInit();
	ilEnable(IL_ORIGIN_SET); 
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	iluInit();
	ilutRenderer(ILUT_OPENGL);
}

bool TgTexture::loadFromFile(const std::string& filename) {
	ilGenImages(1, &m_image);
	ilBindImage(m_image);
	ILboolean result = ilLoadImage(filename.c_str());

	if (result == IL_TRUE) {
		// Convert image to RGBA
		result = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (result == IL_TRUE) {
			// Get the image information
			m_imageData.m_bytes = ilGetData();
			m_imageData.m_bitDepth = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
			m_imageData.m_width = ilGetInteger(IL_IMAGE_WIDTH);
			m_imageData.m_height = ilGetInteger(IL_IMAGE_HEIGHT);
			m_imageData.m_format = ilGetInteger(IL_IMAGE_FORMAT);

			loadFromMemory(m_imageData);
		}
		return GL_TRUE;
	}

	return GL_FALSE;
}

bool TgTexture::loadFromMemory(const TgTextureData& textureData)
{
	glGenTextures(1, &m_textureId);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(m_target, m_textureId);

	if (m_multisampling) 
	{
		glTexImage2DMultisample(m_target, m_samples, m_internalFormat, textureData.m_width, 
			textureData.m_height, GL_TRUE);
	}
	else 
	{
		glTexImage2D(m_target, 0, m_internalFormat, textureData.m_width, textureData.m_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, textureData.m_bytes);

		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_minFilter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_magFilter);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrapU);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_wrapV);
	}

	if (m_minFilter == eTextureFilter::MipMapLinearLinear  ||
		m_minFilter == eTextureFilter::MipMapLinearNearest || 
		m_minFilter == eTextureFilter::MipMapNearestLinear ||
		m_minFilter == eTextureFilter::MipMapNearestNearest) 
	{
		glGenerateMipmap(m_target);

		GLfloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
		glTexParameterf(m_target, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);
	}
	else 
	{
		glTexParameteri(m_target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_target, GL_TEXTURE_MAX_LEVEL, 0);
	}

	glBindTexture(m_target, 0);

	return GL_TRUE;
}

eTextureTarget TgTexture::getTextureTarget() const
{
	return (eTextureTarget)m_target;
}

const TgTextureData& TgTexture::getTextureData() const
{
	return m_imageData;
}

void TgTexture::bind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(m_target, m_textureId);
}

void TgTexture::bind() const
{
	glBindTexture(m_target, m_textureId);
}

void TgTexture::setInternalFormat(eTextureFormat internalFormat)
{
	m_internalFormat = internalFormat;
}

void TgTexture::setFormat(eTextureFormat format)
{
	m_format = format;
}

void TgTexture::generateMipMaps()
{
}

void TgTexture::setFilter(eTextureFilter minFilter, eTextureFilter magFilter)
{
	m_minFilter = minFilter;
	m_magFilter = magFilter;
}

void TgTexture::enableMultisampling(unsigned int samples)
{
	m_target = eTextureTarget::Multisample;
	m_samples = samples;
	m_multisampling = true;
}

void TgTexture::setWrap(eTextureWrap u, eTextureWrap v)
{
	m_wrapU = u;
	m_wrapV = v;
}

TgTexture::~TgTexture()
{
	//ilDeleteImages(1, &m_image);
	glDeleteTextures(1, &m_textureId);
}

unsigned int TgTexture::getTextureId() const
{
	return m_textureId;
}
