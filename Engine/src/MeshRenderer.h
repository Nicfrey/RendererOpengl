#pragma once
#include "Component.h"
#include "GL/glew.h"

class Shader;
class Mesh;

class MeshRenderer: public Component
{
public:
	MeshRenderer();
	MeshRenderer(Shader* pShader, Mesh* pMesh);
	~MeshRenderer() override;
	MeshRenderer(const MeshRenderer& other) = delete;
	MeshRenderer(MeshRenderer&& other) noexcept = delete;
	MeshRenderer& operator=(const MeshRenderer& other) = delete;
	MeshRenderer& operator=(MeshRenderer&& other) noexcept = delete;
	void SetMesh(Mesh* pMesh);
	Mesh* GetMesh() const { return m_pMesh; }
	void Render() const override;
	void RenderGUI() override;
private:
	Shader* m_pShader;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	Mesh* m_pMesh;
};
