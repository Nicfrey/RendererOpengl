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

int Image::GetWidth() const
{
	return m_Width;
}

int Image::GetHeight() const
{
	return m_Height;
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

	int nrChannels;
	stbi_set_flip_vertically_on_load(true);
	const auto data{ stbi_load(path,&m_Width,&m_Height,&nrChannels,0) };
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

		GLint format = nrChannels == 4 ? GL_RGBA : (nrChannels == 3 ? GL_RGB : GL_RED);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		Shader::CheckError("Generate texImage2D Image");
		glGenerateMipmap(GL_TEXTURE_2D);
		Shader::CheckError("GenerateMipMap Image");
	}
	stbi_image_free(data);
}