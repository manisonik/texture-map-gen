#include "stdafx.h"
#include "msk-gl-text.h"

using namespace Manisonik;

MText::MText()
{
	m_pFont = std::make_shared<MFont>("C:\\Windows\\fonts\\arial.ttf");
}

MText::MText(std::shared_ptr<MFont>& font)
{
	SetFont(font);
}

MText::~MText()
{
	m_pFont.reset();
}

void MText::SetFont(std::shared_ptr<MFont>& font)
{
	m_pFont = font;
}

void MText::SetText(std::string text)
{
}
