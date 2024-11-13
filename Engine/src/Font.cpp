#include "Font.h"

#include <iostream>
#include <stdexcept>

#include "Shader.h"
#include "ShaderManager.h"
#include "freetype/internal/ftobjs.h"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"

Shader* Font::m_pShader{nullptr};
GLuint Font::m_VAO{0};
GLuint Font::m_VBO{0};
int Font::m_NumberFont{0};

Font::Font(const char* path, int size) : m_Path(path) , m_Size(size)
{
	if(m_NumberFont++ == 0)
	{
		CreateFontShader();
	}
	LoadFont();
}

Font::~Font()
{
	Cleanup();
}

void Font::Render(const std::string& text, float x, float y, float scale, const glm::vec3& color)
{
	m_pShader->Use();
	m_pShader->SetUniform("textColor", color);
	m_pShader->SetUniform("projection", glm::ortho(0.0f, 800.0f, 0.0f, 600.0f));
	m_pShader->SetUniform("text", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	for(char c: text)
	{
		const Character& character{ m_Characters[c] };

		const float xPos{ x + character.xOffset * scale };
		const float yPos{ y - (character.height - character.yOffset) * scale };

		const float w{ character.width * scale };
		const float h{ character.height * scale };

		const float vertices[6][4] = {
					{xPos, yPos + h, 0.0f, 0.0f},
					{xPos, yPos, 0.0f, 1.0f},
					{xPos + w, yPos, 1.0f, 1.0f},

					{xPos, yPos + h, 0.0f, 0.0f},
					{xPos + w, yPos, 1.0f, 1.0f},
					{xPos + w, yPos + h, 1.0f, 0.0f}
				};

		glBindTexture(GL_TEXTURE_2D, character.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (character.advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned Font::GetTextureID(char c)
{
	return m_Characters[c].textureID;
}

void Font::SetSize(int size)
{
	m_Size = size;
	LoadFont();
}

void Font::LoadFont()
{
	FT_Library library;
	if (FT_Init_FreeType(&library))
	{
		throw std::runtime_error("Failed to initialize FreeType");
	}

	FT_Face face;
	if (FT_New_Face(library, m_Path, 0, &face))
	{
		throw std::runtime_error("Failed to load font face");
	}

	FT_Set_Pixel_Sizes(face, 0, m_Size);
	LoadCharacters(face);

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

void Font::Cleanup()
{
	for (auto& character : m_Characters)
	{
		glDeleteTextures(1, &character.second.textureID);
	}
	m_Characters.clear();
}

void Font::LoadCharacters(FT_Face face)
{
	Cleanup();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(char c = 32; c < 127; c++)
	{
		m_Characters[c] = Character{LoadCharacter(c, face)};
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

Character Font::LoadCharacter(char c, FT_Face face)
{
	if(FT_Load_Char(face, c, FT_LOAD_RENDER))
	{
		throw std::runtime_error("Failed to load glyph");
	}
	unsigned int textureID;
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RED,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0, 
		GL_RED, 
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	return Character{
		textureID,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		face->glyph->bitmap_left,
		face->glyph->bitmap_top,
		static_cast<unsigned>(face->glyph->advance.x)
	};
}

void Font::CreateFontShader()
{
	const char* fragmentShaderSource = R"(
	#version 330 core
	in vec2 TexCoords;
	out vec4 color;

	uniform sampler2D text;
	uniform vec3 textColor;

	void main()
	{    
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
		color = vec4(textColor, 1.0) * sampled;
	}  
	)";

	const char* vertexShaderSource = R"(
	# version 330 core

	layout(location = 0) in vec4 position;
	out vec2 TexCoords;

	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * vec4(position.xy, 0.0, 1.0);
		TexCoords = position.zw;
	}
	)";

	m_pShader = new Shader{vertexShaderSource,fragmentShaderSource,false};
	ShaderManager::GetInstance().AddShader(m_pShader);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
