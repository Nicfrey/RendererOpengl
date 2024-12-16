#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 color;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(model))) * normal;
	TexCoord = texCoord;
	Color = color;
	gl_Position = projection * view * model * vec4(position, 1.0);
}


