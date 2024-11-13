#pragma once
#include <map>
#include <string>

#include "freetype/freetype.h"
#include "gl/glew.h"
#include "glm/vec3.hpp"

class Shader;

struct Character
{
	GLuint textureID;
	unsigned width;
	unsigned height;
	int xOffset;
	int yOffset;
	unsigned int advance;
};

class Font final
{
public:
	Font(const char* path, int fontSize = 32);
	~Font();
	void Render(const std::string& text, float x, float y, float scale, const glm::vec3& color);
	const char* GetPath() const { return m_Path; }
	int GetSize() const { return m_Size; }
	unsigned int GetTextureID(char c);
	void SetSize(int size);	

private:
	void LoadFont();
	void Cleanup();
	void LoadCharacters(FT_Face face);
	Character LoadCharacter(char c, FT_Face face);
	static void CreateFontShader();

	const char* m_Path;
	int m_Size;
	std::map<char, Character> m_Characters;
	static int m_NumberFont;
	static Shader* m_pShader;
	static GLuint m_VAO;
	static GLuint m_VBO;
};
