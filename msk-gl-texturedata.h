#pragma once

namespace TexGen {
	class TgTextureData
	{
	public:
		unsigned int	m_width;
		unsigned int	m_height;
		unsigned char	m_colorType;
		unsigned char	m_bitDepth;
		unsigned char	m_format;
		unsigned int	m_numOfPasses;
		unsigned char* 	m_bytes;
	};
}

