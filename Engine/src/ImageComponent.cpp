#include "ImageComponent.h"
#include <iostream>

#include "Engine.h"
#include "Image.h"
#include "imgui.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "Vertex.h"

Shader* ImageComponent::m_pShader{ nullptr };
GLuint ImageComponent::m_VAO{ 0 };
GLuint ImageComponent::m_VBO{ 0 };
GLuint ImageComponent::m_EBO{ 0 };

ImageComponent::ImageComponent(const char* path)
{
	if(!m_pShader)
	{
		std::cout << "loading shader\n";
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
	m_pShader->SetUniform("projection", glm::ortho(0.0f, static_cast<float>(Engine::GetWindowSize().x), 0.0f,static_cast<float>(Engine::GetWindowSize().y)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pImage->GetTextureID());
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
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
			FragColor = vec4(TexCoord,0.0,1.0);
		}
	)";

	const std::vector<Vertex2D> vertices = {
		{{0.5f,0.5f},{1.f,1.f}},
		{{0.5f,-0.5f},{1.f,0.f}},
		{{-0.5f,-0.5f},{0.f,0.f}},
		{{-0.5f,0.5f},{0.f,1.f}}	
	};

	const std::vector<GLuint> indices = {
			0,1,3,
			1,2,3
	};

	m_pShader = new Shader(vertexShaderSource, fragmentShaderSource,false);
	ShaderManager::GetInstance().AddShader(m_pShader);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE,sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D,position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2D),(GLvoid*)offsetof(Vertex2D,texCoord));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
