#pragma once
#include "ImageComponent.h"
#include "gl/glew.h"

class Shader;

class Image final
{
public:
	Image(const char* path);
	~Image();
	GLuint GetTextureID() const { return m_TextureID; }
	int GetWidth() const;
	int GetHeight() const;
private:
	void Destroy();
	void LoadImage(const char* path);
	GLuint m_TextureID;
	int m_Width;
	int m_Height;
};
