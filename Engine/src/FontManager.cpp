#include "FontManager.h"

#include <iostream>

#include "Font.h"

FontManager::~FontManager()
{
	Destroy();
}

void FontManager::Destroy()
{
	for(auto& font : m_pFonts)
	{
		delete font;
		font = nullptr;
	}
	m_pFonts.clear();
}

void FontManager::AddFont(Font* pFont)
{
	const auto it{ std::ranges::find(m_pFonts, pFont) };
	if(it == m_pFonts.end())
	{
		m_pFonts.push_back(pFont);
	}
}

void FontManager::AddFont(const char* fontPath, int fontSize)
{
	const auto it{ std::ranges::find_if(m_pFonts,[fontPath, fontSize](const Font* pFont)
	{
		return pFont->GetPath() == fontPath && pFont->GetSize() == fontSize;
	}) };
	if(it != m_pFonts.end())
	{
		std::cerr << "Font already exists\n";
		return;
	}
	m_pFonts.push_back(new Font{ fontPath, fontSize });
}

Font* FontManager::GetFont(const char* fontPath) const
{
	const auto it{ std::ranges::find_if(m_pFonts,[fontPath](const Font* pFont)
	{
		return pFont->GetPath() == fontPath;
	}) };
	if(it != m_pFonts.end())
	{
		return *it;
	}
	return nullptr;
}

Font* FontManager::GetFont(int index) const
{
	if(index >= 0 && index < m_pFonts.size())
	{
		return m_pFonts[index];
	}
	return nullptr;
}

