#pragma once
#include "Component.h"
#include "gl/glew.h"

class Shader;
class Image;

class ImageComponent : public Component
{
public:
	ImageComponent(const char* path);
	ImageComponent(const char* path, int width, int height);
	~ImageComponent() override;
	ImageComponent(const ImageComponent& other) = delete;
	ImageComponent(ImageComponent&& other) noexcept = delete;
	ImageComponent& operator=(const ImageComponent& other) = delete;
	ImageComponent& operator=(ImageComponent&& other) noexcept = delete;
	void Render() const override;
	void RenderGUI() override;
private:
	void Destroy();
	void LoadImage(const char* path);
	static void LoadShader();
	Image* m_pImage{ nullptr };
	int m_Width;
	int m_Height;
	static Shader* m_pShader;
	static GLuint m_VAO;
	static GLuint m_VBO;
	static GLuint m_EBO;
};
