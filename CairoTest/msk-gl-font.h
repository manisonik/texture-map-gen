#pragma once
#include "msk-core.h"

class MFont : public MskFont
{
private:
	FT_Library m_pFreeType;
	FT_Face m_pFace;
	GLuint m_nWidth;
	GLuint m_nHeight;
	GLuint m_hTextureId;

	std::vector<MskFontCharacter> m_pCharacterMap;
public:
	MFont();
	MFont(std::string filepath);
	virtual ~MFont();

	std::vector<MskFontCharacter> GetCharacters();
	GLuint GetTextureWidth();
	GLuint GetTextureHeight();
	GLuint GetTextureId();
};

