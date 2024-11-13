#include "ImageComponent.h"
#include <iostream>

#include "Image.h"
#include "imgui.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "glm/ext/matrix_clip_space.hpp"

Shader* ImageComponent::m_pShader{ nullptr };
GLuint ImageComponent::m_VAO{ 0 };
GLuint ImageComponent::m_VBO{ 0 };
GLuint ImageComponent::m_EBO{ 0 };

ImageComponent::ImageComponent(const char* path)
{
	if(!m_pShader)
	{
		LoadShader();
	}
	LoadImage(path);
}

ImageComponent::~ImageComponent()
{
	Destroy();
}

void ImageComponent::Render() const
{
	m_pShader->Use();
	m_pShader->SetUniform("projection", glm::ortho(0.0f, 800.0f, 0.0f, 600.0f));
	m_pShader->SetUniform("texture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pImage->GetTextureID());
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void ImageComponent::RenderGUI()
{
	ImGui::Text("Image Component");
	ImGui::Image((void*)(intptr_t)m_pImage->GetTextureID(), ImVec2(100, 100));
}

void ImageComponent::Destroy()
{
	if(!m_pImage)
	{
		return;
	}
	delete m_pImage;
	m_pImage = nullptr;
}

void ImageComponent::LoadImage(const char* path)
{
	Destroy();
	m_pImage = new Image(path);
}

void ImageComponent::LoadShader()
{
	const char* vertexShaderSource = R"(

		#version 330 core
		layout(location = 0) in vec2 aPos;
		layout(location = 1) in vec2 aTexCoord;

		uniform mat4 projection;
		out vec2 TexCoord;

		void main()
		{
			gl_Position = projection * vec4(aPos, 0.0, 1.0);
			TexCoord = aTexCoord;
		}
	)";

	const char* fragmentShaderSource = R"(

		#version 330 core
		out vec4 FragColor;

		in vec2 TexCoord;

		uniform sampler2D texture1;

		void main()
		{
			FragColor = texture(texture1, TexCoord);
		}
	)";

	constexpr float vertices[] = {
		0.5f,0.5f, 1.0f, 1.0f, // top right
		0.5f,-0.5f, 1.0f, 0.0f, // bottom right
		-0.5f,-0.5f, 0.0f, 0.0f, // bottom left
		-0.5f,0.5f,0.f,1.0f // top left
	};

	const GLuint indices[] = {
			0,1,3,
			1,2,3
	};

	m_pShader = new Shader(vertexShaderSource, fragmentShaderSource,false);
	ShaderManager::GetInstance().AddShader(m_pShader);
	std::cout << glGetError() << std::endl;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
