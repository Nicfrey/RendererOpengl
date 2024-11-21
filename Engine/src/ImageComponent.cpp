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
#include "Mesh.h"

Shader* ImageComponent::m_pShader{ nullptr };
GLuint ImageComponent::m_VAO{ 0 };
GLuint ImageComponent::m_VBO{ 0 };
GLuint ImageComponent::m_EBO{ 0 };

ImageComponent::ImageComponent(const char* path)
{
	LoadImage(path);
	if(!m_pShader)
	{
		std::cout << "loading shader\n";
		LoadShader();
	}
}

ImageComponent::ImageComponent(const char* path, int width, int height): ImageComponent(path)
{
	m_Width = width;
	m_Height = height;
}

ImageComponent::~ImageComponent()
{
	Destroy();
}

void ImageComponent::Render() const
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_CCW);
	m_pShader->Use();
	m_pShader->SetUniform("projection", glm::ortho(0.0f, static_cast<float>(Engine::GetWindowSize().x), 0.0f,static_cast<float>(Engine::GetWindowSize().y)));
	m_pShader->SetUniform("model", GetTransform()->GetModelMatrix());
	m_pShader->SetUniform("texture1", 0);
	glActiveTexture(GL_TEXTURE0);
	Shader::CheckError("Bind Texture ImageComponent");
	glBindVertexArray(m_VAO);
	glBindTexture(GL_TEXTURE_2D, m_pImage->GetTextureID());

	const std::vector<Vertex2D> vertices = {
		{.position = {0.5f * m_Width, 0.5f * m_Height},.texCoord = {1.f, 1.f}},
		{.position = {0.5f * m_Width, -0.5f * m_Height},.texCoord = {1.f, 0.f}},
		{.position = {-0.5f * m_Width, -0.5f * m_Height},.texCoord = {0.f, 0.f}},
		{.position = {-0.5f * m_Width, 0.5f * m_Height},.texCoord = {0.f, 1.f}}
	};


	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex2D), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, nullptr);

	Shader::CheckError("Draw Element ImageComponent");

}

void ImageComponent::RenderGUI()
{
	int size[2] = { m_Width,m_Height };
	ImGui::Text("Image Component");
	ImGui::DragInt2("Size", size,1,1);
	ImGui::Image((void*)(intptr_t)m_pImage->GetTextureID(), ImVec2(100, 100));

	m_Width = size[0];
	m_Height = size[1];
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
	m_Width = m_pImage->GetWidth();
	m_Height = m_pImage->GetHeight();
}

void ImageComponent::LoadShader()
{
	const char* vertexShaderSource = R"(

		#version 330 core
		layout(location = 0) in vec2 aPos;
		layout(location = 1) in vec2 aTexCoord;

		uniform mat4 projection;
		uniform mat4 model;
		out vec2 TexCoord;

		void main()
		{
			gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
			TexCoord = aTexCoord;
		}
	)";

	const char* fragmentShaderSource = R"(

		#version 330 core
		in vec2 TexCoord;
		out vec4 FragColor;

		uniform sampler2D texture1;

		void main()
		{
			FragColor = texture(texture1, TexCoord);
		}
	)";

	const std::vector<Vertex2D> vertices = {
		{.position= {0.5f, 0.5f},.texCoord= {1.f, 1.f}},
		{.position= {0.5f, -0.5f},.texCoord= {1.f, 0.f}},
		{.position= {-0.5f, -0.5f},.texCoord= {0.f, 0.f}},
		{.position= {-0.5f, 0.5f},.texCoord= {0.f, 1.f}}
	};

	const std::vector<unsigned int> indices{0,1,3,1,2,3};

	m_pShader = new Shader(vertexShaderSource, fragmentShaderSource,false);
	ShaderManager::GetInstance().AddShader(m_pShader);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	Shader::CheckError("EBO");

	glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE,sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D,position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2D),(GLvoid*)offsetof(Vertex2D,texCoord));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	Shader::CheckError("Create Shader for image");
	Shader::DebugBuffer(m_VBO, vertices.size() * sizeof(Vertex2D));
	Shader::DebugEBO(m_EBO, indices.size() * sizeof(unsigned int));
}
