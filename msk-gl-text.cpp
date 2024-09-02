#include "stdafx.h"
#include "msk-gl-text.h"

using namespace TexGen;

TgText::TgText() : 
	m_font("C:\\Windows\\fonts\\arial.ttf")
{
}

TgText::TgText(const TgFont& font)
{
	setFont(font);
}

TgText::~TgText()
{
}

void TgText::setFont(const TgFont& font)
{
	m_font = font;
}

void TgText::setText(const std::string& text)
{
}
