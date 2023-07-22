#pragma once
#include "msk-structs.h"

namespace Manisonik {
	class MFont
	{
	private:
		FT_Library m_pFreeType;
		FT_Face m_pFace;
		GLuint m_nWidth;
		GLuint m_nHeight;
		GLuint m_hTextureId;

		std::vector<SFontCharacter> m_pCharacterMap;
	public:
		MFont();
		MFont(std::string filepath);
		virtual ~MFont();

		std::vector<SFontCharacter> GetCharacters();
		uint32_t GetTextureWidth();
		uint32_t GetTextureHeight();
		uint32_t GetTextureId();
	};
}

