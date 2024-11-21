#include "MeshRenderer.h"

#include "Camera.h"
#include "GameObject.h"
#include "imgui.h"
#include "Light.h"
#include "Mesh.h"
#include "Shader.h"

MeshRenderer::MeshRenderer()
	: m_pShader(nullptr), m_VAO(0), m_VBO(0), m_EBO(0), m_pMesh(nullptr)
{
	SetMesh(m_pMesh);
}

MeshRenderer::MeshRenderer(Shader* pShader, Mesh* pMesh): m_pShader(pShader), m_VAO(0), m_VBO(0), m_EBO(0), m_pMesh(pMesh)
{
	SetMesh(m_pMesh);
}

MeshRenderer::~MeshRenderer()
{
	if(m_VAO)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
	if(m_VBO)
	{
		glDeleteBuffers(1, &m_VBO);
	}
	if(m_EBO)
	{
		glDeleteBuffers(1, &m_EBO);
	}
	if(m_pMesh)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	m_pShader = nullptr;
}

void MeshRenderer::SetMesh(Mesh* pMesh)
{
	m_pMesh = pMesh;

	if(!m_pMesh)
	{
		return;
	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_pMesh->GetVertices().size() * sizeof(Vertex), m_pMesh->GetVertices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pMesh->GetIndices().size() * sizeof(GLuint), m_pMesh->GetIndices().data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,texCoord));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,normal));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,color));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void MeshRenderer::Render() const
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	const Camera* pCamera = Camera::GetMainCamera();
	if(!m_pMesh || !m_pShader || !pCamera)
	{
		return;
	}
	m_pShader->Use();

	m_pShader->SetUniform("model",GetGameObject()->GetTransform()->GetModelMatrix());
	m_pShader->SetUniform("view",pCamera->GetViewMatrix());
	m_pShader->SetUniform("projection",pCamera->GetProjectionMatrix());

	m_pShader->SetUniform("lightDir",Light::GetDirectionalLight()->GetTransform()->GetForward());
	m_pShader->SetUniform("intensity",Light::GetDirectionalLight()->m_Intensity);


	// Render mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_pMesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void MeshRenderer::RenderGUI()
{
	ImGui::Text("MeshRenderer");
	if(m_pMesh)
	{
		ImGui::Text("Vertices: %lu", m_pMesh->GetVertices().size());
		ImGui::Text("Indices: %lu", m_pMesh->GetIndices().size());
	}
	else
	{
		ImGui::Text("No mesh");
	}
}
