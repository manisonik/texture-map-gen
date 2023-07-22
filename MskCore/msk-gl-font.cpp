#include "stdafx.h"
#include "msk-gl-font.h"

using namespace Manisonik;

MFont::MFont() :
	m_pFace(0),
	m_pFreeType(0)
{
}

MFont::MFont(std::string filepath)
{
	GLuint w = 0, h = 0, x = 0;

	// Load the font
	if (FT_Init_FreeType(&m_pFreeType)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	if (FT_New_Face(m_pFreeType, filepath.c_str(), 0, &m_pFace)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(m_pFace, 14, 0);

	// Calculate max width and height
	for (int i = 0; i < 128; i++) {
		if (FT_Load_Char(m_pFace, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		w += m_pFace->glyph->bitmap.width;
		h = std::max(h, m_pFace->glyph->bitmap.rows);
	}

	// Store width and height
	m_nWidth = w;
	m_nHeight = h;

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_hTextureId);
	glBindTexture(GL_TEXTURE_2D, m_hTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the ASCII characters
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		FT_Error error = FT_Load_Char(m_pFace, c, FT_LOAD_RENDER);
		if (error > 0)
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// Generate texture
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			x,
			0,
			m_pFace->glyph->bitmap.width,
			m_pFace->glyph->bitmap.rows,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_pFace->glyph->bitmap.buffer
		);

		// Now store character for later use
		SFontCharacter character = {
			m_pFace->glyph->advance.x >> 6,
			m_pFace->glyph->advance.y >> 6,
			m_pFace->glyph->bitmap.width,
			m_pFace->glyph->bitmap.rows,
			m_pFace->glyph->bitmap_left,
			m_pFace->glyph->bitmap_top,
			(float)x / w
		};

		m_pCharacterMap.push_back(character);

		// Increment
		x += m_pFace->glyph->bitmap.width;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

MFont::~MFont()
{
	FT_Done_Face(m_pFace);
	FT_Done_FreeType(m_pFreeType);
}

std::vector<SFontCharacter> MFont::GetCharacters()
{
	return m_pCharacterMap;
}

GLuint MFont::GetTextureWidth()
{
	return m_nWidth;
}

GLuint MFont::GetTextureHeight()
{
	return m_nHeight;
}

GLuint MFont::GetTextureId()
{
	return m_hTextureId;
}