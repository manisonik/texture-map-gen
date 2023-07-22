#pragma once

namespace Manisonik {
	class MTextureData
	{
	public:
		GLuint		m_nWidth;
		GLuint		m_nHeight;
		GLubyte		m_nColorType;
		GLubyte		m_nBitDepth;
		GLubyte		m_nFormat;
		GLuint		m_nNumOfPasses;
		GLubyte* m_pBytes;
	};
}

