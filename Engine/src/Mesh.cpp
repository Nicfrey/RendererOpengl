#include "Mesh.h"

#include <stdexcept>
#include <GL/glew.h>

#include "MathHelper.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

std::vector<Vertex> Mesh::GetVertices() const
{
	return m_Vertices;
}

std::vector<GLuint> Mesh::GetIndices() const
{
	return m_Indices;
}

void Mesh::SetColor(const glm::vec3& color)
{
	for(Vertex& vertex : m_Vertices)
	{
		vertex.color = color;
	}
}

Mesh* Mesh::CreateCube()
{
	Mesh* pMesh = new Mesh();

	glm::vec2 texCoords[] = {
		// Face avant
		{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
		// Face arrière
		{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
		// Face droite
		{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
		// Face gauche
		{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
		// Face supérieure
		{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
		// Face inférieure
		{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
	};

	glm::vec3 vertices[] = {
		// Face avant
		{-1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f},
		// Face arrière
		{-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f},
		// Face droite
		{ 1.0f, -1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f},
		// Face gauche
		{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f, -1.0f},
		// Face supérieure
		{-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},
		// Face inférieure
		{-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f}
	};

	glm::vec3 normals[] = {
		// Normales des faces
		{ 0.0f,  0.0f,  1.0f}, // Face avant
		{ 0.0f,  0.0f, -1.0f}, // Face arrière
		{ 1.0f,  0.0f,  0.0f}, // Face droite
		{-1.0f,  0.0f,  0.0f}, // Face gauche
		{ 0.0f,  1.0f,  0.0f}, // Face supérieure
		{ 0.0f, -1.0f,  0.0f}, // Face inférieure
	};

	for (int i = 0; i < 24; ++i)
	{
		Vertex vertex{};
		vertex.position = vertices[i];
		vertex.normal = normals[i / 4]; // Chaque face a 4 sommets
		vertex.texCoord = texCoords[i];
		vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
		pMesh->m_Vertices.push_back(vertex);
	}

	// Indices pour chaque face
	pMesh->m_Indices = {
		0, 1, 2, 0, 2, 3,
		// Back face
		4, 5, 6, 4, 6, 7,
		// Left face
		8, 9, 10, 8, 10, 11,
		// Right face
		12, 13, 14, 12, 14, 15,
		// Top face
		16, 17, 18, 16, 18, 19,
		// Bottom face
		20, 21, 22, 20, 22, 23,
	};

	return pMesh;
}

Mesh* Mesh::CreatePlane()
{
	Mesh* pMesh = new Mesh();
	pMesh->m_Vertices = {
		{{-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}, // Bas-gauche
		{{ 0.5f, 0.0f, -0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}, // Bas-droite
		{{ 0.5f, 0.0f,  0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}, // Haut-droite
		{{-0.5f, 0.0f,  0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}  // Haut-gauche
	};
	pMesh->m_Indices = {
		0, 1, 2,
		2, 3, 0
	};
	return pMesh;
}

Mesh* Mesh::CreateSphere(unsigned int stacks,unsigned int slices)
{
	Mesh* pMesh = new Mesh();
	const float sliceStep{ 2 * MathHelper::PI / slices };
	const float stackStep{ MathHelper::PI / stacks };

	for(unsigned int i{}; i <= stacks; ++i)
	{
		const float stackAngle{ MathHelper::PI / 2 - i * stackStep };
		const float xy{ MathHelper::Cos(stackAngle) };
		const float z{ MathHelper::Sin(stackAngle) };

		for(unsigned int j{}; j <= slices; ++j)
		{
			const float sliceAngle{ static_cast<float>(j) * sliceStep };

			const float x{ xy * MathHelper::Cos(sliceAngle) };
			const float y{ xy * MathHelper::Sin(sliceAngle) };
			pMesh->m_Vertices.emplace_back(Vertex{
				{x,y,z},
				{static_cast<float>(j) / slices, static_cast<float>(i) / stacks},
				{x,y,z},
				{1.f,1.f,1.f},
			});
		}
	}

	for (unsigned int i = 0; i < stacks; ++i) {
		for (unsigned int j = 0; j < slices; ++j) {
			// Calcul des indices pour le quad courant
			unsigned int first = (i * (slices + 1)) + j;
			unsigned int second = first + slices + 1;

			// Triangle 1
			pMesh->m_Indices.push_back(first);
			pMesh->m_Indices.push_back(second);
			pMesh->m_Indices.push_back(first + 1);

			// Triangle 2
			pMesh->m_Indices.push_back(second);
			pMesh->m_Indices.push_back(second + 1);
			pMesh->m_Indices.push_back(first + 1);
		}
	}
	return pMesh;
}

Mesh* Mesh::CreateCylinder(float radius, float height, unsigned slices)
{
	Mesh* pMesh = new Mesh();

	const float halfHeight{ height / 2.f };
	const float sliceStep{ 2 * MathHelper::PI / slices };

	for(unsigned i{}; i <= slices; ++i)
	{
		const float angle{ i * sliceStep };
		const float x{ radius * MathHelper::Cos(angle) };
		const float z{ radius * MathHelper::Sin(angle) };

		pMesh->m_Vertices.emplace_back(Vertex{
			{x, -halfHeight, z},
			{static_cast<float>(i) / slices, 0.f},
			{x, 0.f, z},
			{1.f, 1.f, 1.f}
			});

		pMesh->m_Vertices.emplace_back(Vertex{
			{x, halfHeight, z},
			{static_cast<float>(i) / slices, 1.f},
			{x, 0.f, z},
			{1.f, 1.f, 1.f}
		});
	}

	for(unsigned i{}; i < slices; ++i)
	{
		const unsigned k1{ i * 2 };
		const unsigned k2{ k1 + 1 };

		pMesh->m_Indices.push_back(k1);
		pMesh->m_Indices.push_back(k1 + 2);
		pMesh->m_Indices.push_back(k2);

		pMesh->m_Indices.push_back(k2);
		pMesh->m_Indices.push_back(k1 + 2);
		pMesh->m_Indices.push_back(k2 + 2);
	}

	const unsigned topCenterIndex{ slices * 2 };
	for(unsigned i{}; i < slices; ++i)
	{
		pMesh->m_Indices.push_back(topCenterIndex);
		pMesh->m_Indices.push_back(i * 2 + 1);
		pMesh->m_Indices.push_back(((i + 1) % slices) * 2 + 1);
	}

	const unsigned bottomCenterIndex{ topCenterIndex + 1 };
	for(unsigned i{}; i < slices; ++i)
	{
		pMesh->m_Indices.push_back(bottomCenterIndex);
		pMesh->m_Indices.push_back(((i + 1) % slices) * 2);
		pMesh->m_Indices.push_back(i * 2);
	}

	return pMesh;
}

void Mesh::CalculateNormals(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	for(size_t i{}; i < indices.size(); ++i)
	{
		Vertex& v0{ vertices[indices[i]] };
		Vertex& v1{ vertices[indices[i + 1]] };
		Vertex& v2{ vertices[indices[i + 2]] };

		glm::vec3 edge1{ v1.position - v0.position };
		glm::vec3 edge2{ v2.position - v0.position };
		const glm::vec3 normal{ normalize(cross(edge1, edge2)) };

		v0.normal += normal;
		v1.normal += normal;
		v2.normal += normal;
	}
}

Mesh* Mesh::LoadFromFBX(const char* path)
{
	Assimp::Importer importer;
	const aiScene* scene{ importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices) };

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::runtime_error{ "Failed to load fbx file" };
	}

	aiMesh* mesh = scene->mMeshes[0];

	Mesh* pMesh{new Mesh{}};

	for (unsigned int i{}; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.position = glm::vec3{ mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z };

		if (mesh->HasNormals())
		{
			vertex.normal = glm::vec3{ mesh->mNormals[i].x,mesh->mNormals[i].y ,mesh->mNormals[i].z };
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord = glm::vec2{ mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y };
		}
		else
		{
			vertex.texCoord = glm::vec2{ 0.f,0.f };
		}

		pMesh->m_Vertices.emplace_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			pMesh->m_Indices.emplace_back(face.mIndices[j]);
		}
	}

	// Calculate normals if no normals
	if (!mesh->HasNormals())
	{
		CalculateNormals(pMesh->m_Vertices, pMesh->m_Indices);
	}

	return pMesh;
}
