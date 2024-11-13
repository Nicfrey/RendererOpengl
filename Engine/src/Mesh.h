#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"

class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh& mesh) = delete;
	Mesh(Mesh&& mesh) noexcept = delete;
	Mesh& operator=(const Mesh& mesh) = delete;
	Mesh& operator=(Mesh&& mesh) noexcept = delete;
	~Mesh() = default;
	std::vector<Vertex> GetVertices() const;
	std::vector<GLuint> GetIndices() const;
	void SetColor(const glm::vec3& color);
	static Mesh* CreateCube();
	static Mesh* CreatePlane();
	static Mesh* CreateSphere(unsigned stacks = 32, unsigned slices = 32);
	static Mesh* CreateCylinder(float radius = 1.f, float height = 2.f, unsigned slices = 32);
	static void CalculateNormals(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, std::vector<GLfloat>& normals);
private:
	std::vector<Vertex> m_Vertices;
	std::vector<GLuint> m_Indices;
};