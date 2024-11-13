#version 410 core

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

uniform vec3 lightDir;
uniform float intensity;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 light = normalize(lightDir);
	float diff = max(dot(norm, light), 0.0);
	vec3 diffuse = diff * intensity * Color;
	FragColor = vec4(diffuse,1.0f);
}