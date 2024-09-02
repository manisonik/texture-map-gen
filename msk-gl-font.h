#pragma once
#include "msk-structs.h"

namespace TexGen {
	class TgFont
	{
	private:
		FT_Library m_freeType;
		FT_Face m_face;
		unsigned int  m_width;
		unsigned int  m_height;
		unsigned int  m_textureId;

		std::vector<std::shared_ptr<FontCharacter>> m_characterMap;
	public:
		TgFont();
		TgFont(const std::string& filepath);
		virtual ~TgFont();

		const std::vector<std::shared_ptr<FontCharacter>>& getCharacters() const;
		unsigned int getTextureWidth() const;
		unsigned int  getTextureHeight() const;
		unsigned int  getTextureId() const;
	};
}

