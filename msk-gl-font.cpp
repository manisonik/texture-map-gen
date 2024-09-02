#include "stdafx.h"
#include "msk-gl-font.h"

using namespace TexGen;

TgFont::TgFont() :
	m_face(0),
	m_freeType(0)
{
}

TgFont::TgFont(const std::string& filepath)
{
	GLuint w = 0, h = 0, x = 0;

	// Load the font
	if (FT_Init_FreeType(&m_freeType)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	if (FT_New_Face(m_freeType, filepath.c_str(), 0, &m_face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(m_face, 14, 0);

	// Calculate max width and height
	for (int i = 0; i < 128; i++) {
		if (FT_Load_Char(m_face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		w += m_face->glyph->bitmap.width;
		h = std::max(h, m_face->glyph->bitmap.rows);
	}

	// Store width and height
	m_width = w;
	m_height = h;

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
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
		FT_Error error = FT_Load_Char(m_face, c, FT_LOAD_RENDER);
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
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);

		// Now store character for later use
		auto character = std::make_shared<FontCharacter>(
			m_face->glyph->advance.x >> 6,
			m_face->glyph->advance.y >> 6,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			m_face->glyph->bitmap_left,
			m_face->glyph->bitmap_top,
			(float)x / w
			);

		m_characterMap.push_back(character);

		// Increment
		x += m_face->glyph->bitmap.width;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

TgFont::~TgFont()
{
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_freeType);
}

const std::vector<std::shared_ptr<FontCharacter>>& TgFont::getCharacters() const
{
	return m_characterMap;
}

unsigned int TgFont::getTextureWidth() const
{
	return m_width;
}

unsigned int TgFont::getTextureHeight() const
{
	return m_height;
}

unsigned int TgFont::getTextureId() const
{
	return m_textureId;
}