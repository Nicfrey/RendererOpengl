#pragma once
#include <string>
#include <vector>

#include "Singleton.h"

class Font;

class FontManager : public Singleton<FontManager>
{
public:
	FontManager() = default;
	~FontManager() override;
	FontManager(const FontManager& other) = delete;
	FontManager(FontManager&& other) noexcept = delete;
	FontManager& operator=(const FontManager& other) = delete;
	FontManager& operator=(FontManager&& other) noexcept = delete;
	void AddFont(Font* pFont);
	void AddFont(const char* fontPath, int fontSize = 32);
	Font* GetFont(const char* fontPath) const;
	Font* GetFont(int index) const;
	void Destroy();
private:
	std::vector<Font*> m_pFonts;
};
