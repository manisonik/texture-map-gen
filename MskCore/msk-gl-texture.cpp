#include "stdafx.h"
#include "msk-enum.h"
#include "msk-gl-texture.h"

using namespace Manisonik;

MTexture::MTexture() :
	m_nMinFilter(MskTextureFilter::Nearest),
	m_nMagFilter(MskTextureFilter::Nearest),
	m_nWrapU(MskTextureWrap::ClampToEdge),
	m_nWrapV(MskTextureWrap::ClampToEdge),
	m_nInternalFormat(MskTextureFormat::RGBA),
	m_nFormat(MskTextureFormat::BGRA),
	m_nTarget(MskTextureTarget::TwoDimension),
	m_bMultisampling(false)
{
	ilInit();
	ilEnable(IL_ORIGIN_SET); 
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	iluInit();
	ilutRenderer(ILUT_OPENGL);
}

GLboolean MTexture::LoadFromFile(std::string filename) {
	ilGenImages(1, &m_pImage);
	ilBindImage(m_pImage);
	ILboolean result = ilLoadImage(filename.c_str());

	if (result == IL_TRUE) {
		//Convert image to RGBA
		result = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (result == IL_TRUE) {
			// Get the image information
			auto imageData = std::make_shared<MTextureData>();
			imageData->m_pBytes = ilGetData();
			imageData->m_nBitDepth = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
			imageData->m_nWidth = ilGetInteger(IL_IMAGE_WIDTH);
			imageData->m_nHeight = ilGetInteger(IL_IMAGE_HEIGHT);
			imageData->m_nFormat = ilGetInteger(IL_IMAGE_FORMAT);

			LoadFromMemory(imageData);
		}
		return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean MTexture::LoadFromMemory(std::shared_ptr<MTextureData> textureData)
{
	glGenTextures(1, &m_nTextureId);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(m_nTarget, m_nTextureId);

	// Store image data
	m_pImageData = textureData;

	if (m_bMultisampling) 
	{
		glTexImage2DMultisample(m_nTarget, m_nSamples, m_nInternalFormat, textureData->m_nWidth, 
			textureData->m_nHeight, GL_TRUE);
	}
	else 
	{
		glTexImage2D(m_nTarget, 0, m_nInternalFormat, textureData->m_nWidth, textureData->m_nHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, textureData->m_pBytes);

		glTexParameteri(m_nTarget, GL_TEXTURE_MIN_FILTER, m_nMinFilter);
		glTexParameteri(m_nTarget, GL_TEXTURE_MAG_FILTER, m_nMagFilter);
		glTexParameteri(m_nTarget, GL_TEXTURE_WRAP_S, m_nWrapU);
		glTexParameteri(m_nTarget, GL_TEXTURE_WRAP_T, m_nWrapV);
	}

	if (m_nMinFilter == MskTextureFilter::MipMapLinearLinear  ||
		m_nMinFilter == MskTextureFilter::MipMapLinearNearest || 
		m_nMinFilter == MskTextureFilter::MipMapNearestLinear ||
		m_nMinFilter == MskTextureFilter::MipMapNearestNearest) 
	{
		glGenerateMipmap(m_nTarget);

		GLfloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
		glTexParameterf(m_nTarget, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);
	}
	else 
	{
		glTexParameteri(m_nTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_nTarget, GL_TEXTURE_MAX_LEVEL, 0);
	}

	glBindTexture(m_nTarget, 0);

	return GL_TRUE;
}

MskTextureTarget MTexture::GetTextureTarget()
{
	return (MskTextureTarget)m_nTarget;
}

std::shared_ptr<MTextureData>& MTexture::GetTextureData()
{
	return m_pImageData;
}

void MTexture::Bind(GLuint unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(m_nTarget, m_nTextureId);
}

void MTexture::Bind()
{
	glBindTexture(m_nTarget, m_nTextureId);
}

GLvoid MTexture::SetInternalFormat(MskTextureFormat internalFormat)
{
	m_nInternalFormat = internalFormat;
}

GLvoid MTexture::SetFormat(MskTextureFormat format)
{
	m_nFormat = format;
}

void MTexture::GenerateMipMaps()
{
}

void MTexture::SetFilter(MskTextureFilter minFilter, MskTextureFilter magFilter)
{
	m_nMinFilter = minFilter;
	m_nMagFilter = magFilter;
}

GLvoid MTexture::EnableMultisampling(GLuint samples)
{
	m_nTarget = MskTextureTarget::Multisample;
	m_nSamples = samples;
	m_bMultisampling = true;
}

void MTexture::SetWrap(MskTextureWrap u, MskTextureWrap v)
{
	m_nWrapU = u;
	m_nWrapV = v;
}

MTexture::~MTexture()
{
	ilDeleteImages(1, &m_pImage);
	glDeleteTextures(1, &m_nTextureId);
}

GLuint MTexture::GetTextureId()
{
	return m_nTextureId;
}
