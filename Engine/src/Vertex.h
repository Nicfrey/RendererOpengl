#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec3 color;
};

struct Vertex2D
{
	glm::vec2 position;
	glm::vec2 texCoord;
};