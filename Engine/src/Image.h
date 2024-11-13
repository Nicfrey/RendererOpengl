#pragma once
#include "gl/glew.h"
#include "glm/vec2.hpp"

class Shader;

class Image final
{
public:
	Image(const char* path);
	~Image();
	void Render(const glm::vec2& position, const glm::vec2& dimension) const;
	GLuint GetTextureID() const { return m_TextureID; }
private:
	void Destroy();
	void LoadImage(const char* path);
	GLuint m_TextureID;
};
