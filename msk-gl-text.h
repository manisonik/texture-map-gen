#pragma once
#include "msk-gl-font.h"

namespace TexGen {
	class TgText
	{
	private:
		TgFont m_font;

	public:
		TgText();
		TgText(const TgFont& font);
		~TgText();

		void setFont(const TgFont& font);
		void setText(const std::string& text);
	};
}

