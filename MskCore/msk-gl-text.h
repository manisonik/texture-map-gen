#pragma once
#include "msk-gl-font.h"

namespace Manisonik {
	class MText
	{
	private:
		std::shared_ptr<MFont> m_pFont;

	public:
		MText();
		MText(std::shared_ptr<MFont>& font);
		~MText();

		void SetFont(std::shared_ptr<MFont>& font);
		void SetText(std::string text);
	};
}

