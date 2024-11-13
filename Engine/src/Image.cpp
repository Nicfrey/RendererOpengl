#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

#include "Shader.h"

Image::Image(const char* path)
{
	LoadImage(path);
}

Image::~Image()
{
	Destroy();
}

void Image::Destroy()
{
	if(m_TextureID == 0)
	{
		return;
	}
	glDeleteTextures(1, &m_TextureID);
}

void Image::LoadImage(const char* path)
{
	Destroy();
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	int width;
	int height;
	int nrChannels;
	const auto data{ stbi_load(path,&width,&height,&nrChannels,0) };
	if (!data)
	{
		std::cerr << "Failed to load texture\n";
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
}